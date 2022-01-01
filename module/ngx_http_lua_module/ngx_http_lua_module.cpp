#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "param.hpp"

#include "ngx_http_lua_module.hpp"

static void *ngx_http_lua_create_conf(ngx_conf_t *cf);
static char *ngx_http_lua_load(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_lua_handler(ngx_http_request_t *r);

ngx_http_module_t ngx_http_lua_module_ctx = {
    NULL,                     /* preconfiguration */
    NULL,                     /* postconfiguration */
    NULL,                     /* create main configuration */
    NULL,                     /* init main configuration */
    NULL,                     /* create server configuration */
    NULL,                     /* merge server configuration */
    ngx_http_lua_create_conf, /* create location configuration */
    NULL                      /* merge location configuration */
};

static ngx_command_t ngx_http_lua_commands[] = {
    {ngx_string("lua_load"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_NOARGS,
     ngx_http_lua_load,
     NGX_HTTP_LOC_CONF_OFFSET,
     0,
     NULL},
    {ngx_string("lua_package_search_path"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_lua_loc_conf_t, package_search_path),
     NULL},
    {ngx_string("lua_cpackage_search_path"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_lua_loc_conf_t, cpackage_search_path),
     NULL},
    {ngx_string("lua_uri_pattern"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_lua_loc_conf_t, uri_pattern),
     NULL},
    {ngx_string("lua_expires"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_sec_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_lua_loc_conf_t, expires),
     NULL},
    {ngx_string("lua_memory_limit"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_num_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_lua_loc_conf_t, memory_limit),
     NULL},
    ngx_null_command};

ngx_module_t ngx_http_lua_module = {
    NGX_MODULE_V1,
    &ngx_http_lua_module_ctx, /* module context */
    ngx_http_lua_commands,    /* module directives */
    NGX_HTTP_MODULE,          /* module type */
    NULL,                     /* init master */
    NULL,                     /* init module */
    NULL,                     /* init process */
    NULL,                     /* init thread */
    NULL,                     /* exit thread */
    NULL,                     /* exit process */
    NULL,                     /* exit master */
    NGX_MODULE_V1_PADDING};

static void *ngx_http_lua_create_conf(ngx_conf_t *cf)
{
    ngx_http_lua_loc_conf_t *conf = (ngx_http_lua_loc_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_lua_loc_conf_t));
    if (conf)
    {
        conf->expires = NGX_CONF_UNSET;
        conf->package_search_path.data = NULL;
        conf->package_search_path.len = 0;
        conf->cpackage_search_path.data = NULL;
        conf->cpackage_search_path.len = 0;
        conf->uri_pattern.data = NULL;
        conf->uri_pattern.len = 0;
        conf->memory_limit = NGX_CONF_UNSET;
        return conf;
    }

    return NGX_CONF_ERROR;
}

static ngx_int_t ngx_http_lua_handler(ngx_http_request_t *r)
{
    ngx_http_lua_loc_conf_t *conf = (ngx_http_lua_loc_conf_t *)ngx_http_get_module_loc_conf(r, ngx_http_lua_module);

    if (r->headers_in.if_modified_since && r->headers_in.if_modified_since->value.data)
    {
        time_t now = time(NULL), old = ngx_http_parse_time(r->headers_in.if_modified_since->value.data, r->headers_in.if_modified_since->value.len);
        if (difftime(now, old) <= conf->expires)
        {
            return NGX_HTTP_NOT_MODIFIED;
        }
    }

    hi::request req;
    hi::response res;

    req.uri.assign((char *)r->uri.data, r->uri.len);

    if (lua_uri_re == nullptr)
    {
        lua_uri_re = std::make_shared<std::regex>(std::string((char *)conf->uri_pattern.data, conf->uri_pattern.len));
    }

    if (!std::regex_match(req.uri, *lua_uri_re))
    {
        return NGX_HTTP_BAD_REQUEST;
    }

    hi::get_input_headers(r, req.headers);

    req.method.assign((char *)r->method_name.data, r->method_name.len);
    req.client.assign((char *)r->connection->addr_text.data, r->connection->addr_text.len);

    ngx_table_elt_t *ua = r->headers_in.user_agent;

    if (ua)
    {
        req.user_agent.assign((char *)ua->value.data, ua->value.len);
    }
    if (r->args.len > 0)
    {
        hi::parser_param(req.param, req.form);
    }

    if (r->headers_in.content_length_n > 0)
    {
        std::string input_body = std::move(hi::get_input_body(r));
        ngx_str_t body = ngx_null_string;
        body.data = (u_char *)input_body.c_str();
        body.len = input_body.size();
        if (ngx_strncasecmp(r->headers_in.content_type->value.data, (u_char *)FORM_MULTIPART_TYPE,
                            FORM_MULTIPART_TYPE_LEN) == 0)
        {
            ngx_http_core_loc_conf_t *clcf = (ngx_http_core_loc_conf_t *)ngx_http_get_module_loc_conf(r, ngx_http_core_module);
            std::string upload_err_msg;
            if (!hi::upload(req, input_body, clcf, r, "temp", upload_err_msg))
            {
                res.content = std::move(upload_err_msg);
                res.status = 500;
                return hi::set_output_headers_body(r, res);
            }
        }
        else if (ngx_strncasecmp(r->headers_in.content_type->value.data, (u_char *)FORM_URLENCODED_TYPE, FORM_URLENCODED_TYPE_LEN) == 0)
        {
            hi::parser_param(std::string((char *)body.data, body.len), req.form);
        }
    }

    if (r->headers_in.cookies.elts != NULL && r->headers_in.cookies.nelts != 0)
    {
        ngx_table_elt_t **cookies = (ngx_table_elt_t **)r->headers_in.cookies.elts;
        for (size_t i = 0; i < r->headers_in.cookies.nelts; ++i)
        {
            if (cookies[i]->value.data != NULL)
            {
                hi::parser_param(std::string((char *)cookies[i]->value.data, cookies[i]->value.len), req.cookies, ';');
            }
        }
    }

    if (lua_engine == nullptr)
    {
        lua_engine = std::make_shared<hi::lua>(std::string((char *)conf->package_search_path.data, conf->package_search_path.len), std::string((char *)conf->cpackage_search_path.data, conf->cpackage_search_path.len), conf->memory_limit);
    }

    lua_engine->main(req, res);

    return hi::set_output_headers_body(r, res);
}

static char *ngx_http_lua_load(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *)ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_lua_handler;
    return NGX_CONF_OK;
}
