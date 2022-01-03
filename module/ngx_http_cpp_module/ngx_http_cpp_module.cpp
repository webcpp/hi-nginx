#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "param.hpp"
#include "cache_t.hpp"

#include "ngx_http_cpp_module.hpp"

static void *ngx_http_cpp_create_conf(ngx_conf_t *cf);
static char *ngx_http_cpp_load(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r);

ngx_http_module_t ngx_http_cpp_module_ctx = {
    NULL,                     /* preconfiguration */
    NULL,                     /* postconfiguration */
    NULL,                     /* create main configuration */
    NULL,                     /* init main configuration */
    NULL,                     /* create server configuration */
    NULL,                     /* merge server configuration */
    ngx_http_cpp_create_conf, /* create location configuration */
    NULL                      /* merge location configuration */
};

static ngx_command_t ngx_http_cpp_commands[] = {
    {ngx_string("cpp_load"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_NOARGS,
     ngx_http_cpp_load,
     NGX_HTTP_LOC_CONF_OFFSET,
     0,
     NULL},
    {ngx_string("cpp_search_path"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_cpp_loc_conf_t, search_path),
     NULL},
    {ngx_string("cpp_uri_pattern"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_cpp_loc_conf_t, uri_pattern),
     NULL},
    {ngx_string("cpp_expires"),
     NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_sec_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_cpp_loc_conf_t, expires),
     NULL},
    ngx_null_command};

ngx_module_t ngx_http_cpp_module = {
    NGX_MODULE_V1,
    &ngx_http_cpp_module_ctx, /* module context */
    ngx_http_cpp_commands,    /* module directives */
    NGX_HTTP_MODULE,          /* module type */
    NULL,                     /* init master */
    NULL,                     /* init module */
    NULL,                     /* init process */
    NULL,                     /* init thread */
    NULL,                     /* exit thread */
    NULL,                     /* exit process */
    NULL,                     /* exit master */
    NGX_MODULE_V1_PADDING};

static void *ngx_http_cpp_create_conf(ngx_conf_t *cf)
{
    ngx_http_cpp_loc_conf_t *conf = (ngx_http_cpp_loc_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_cpp_loc_conf_t));
    if (conf)
    {
        conf->expires = NGX_CONF_UNSET;
        conf->search_path.data = NULL;
        conf->search_path.len = 0;
        conf->uri_pattern.data = NULL;
        conf->uri_pattern.len = 0;
        return conf;
    }

    return NGX_CONF_ERROR;
}

static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r)
{
    ngx_http_cpp_loc_conf_t *conf = (ngx_http_cpp_loc_conf_t *)ngx_http_get_module_loc_conf(r, ngx_http_cpp_module);

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

    if (cpp_uri_re == nullptr)
    {
        cpp_uri_re = std::make_shared<std::regex>(std::string((char *)conf->uri_pattern.data, conf->uri_pattern.len));
    }

    if (!std::regex_match(req.uri, *cpp_uri_re))
    {
        return NGX_HTTP_BAD_REQUEST;
    }

    std::string lru_cache_key;

    if (hi::set_output_headers_body_init(r, req, res, conf->expires, std::ref(lru_cache_key)) == NGX_DONE)
    {
        goto done;
    }

    if (cpp_engine == nullptr)
    {
        cpp_engine = std::make_shared<hi::cpp>(std::string((char *)conf->search_path.data, conf->search_path.len));
    }

    cpp_engine->main(req, res);
done:
    return hi::set_output_headers_body(r, res, conf->expires, lru_cache_key);
}

static char *ngx_http_cpp_load(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *)ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_cpp_handler;
    return NGX_CONF_OK;
}
