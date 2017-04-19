extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include "include/request.hpp"
#include "include/response.hpp"
#include "lib/cpp_tools.hpp"


static hi::cpp_tools_t CPP_TOOLS;

typedef struct {
    ngx_str_t module_dir;
    ngx_str_t class_name;
} ngx_http_cpp_loc_conf_t;



static void * ngx_http_cpp_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_cpp_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);
static char *ngx_http_cpp_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r);
static void ngx_http_cpp_body_handler(ngx_http_request_t* r);
static ngx_int_t ngx_http_cpp_normal_handler(ngx_http_request_t *r);


static void get_input_headers(ngx_http_request_t* r, std::map<std::string, std::string>& input_headers);
static void set_output_headers(ngx_http_request_t* r, std::multimap<std::string, std::string>& output_headers);

static void get_input_cookies(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& cookies);
static void set_output_cookies(const std::map<std::string, std::string>& cookies, std::multimap<std::string, std::string>& output_headers, bool ssl);



ngx_command_t ngx_http_cpp_commands[] = {
    {
        ngx_string("cpp_module_dir"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, module_dir),
        NULL
    },
    {
        ngx_string("cpp_call"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_http_cpp_conf_handler,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, class_name),
        NULL
    },

    ngx_null_command
};


ngx_http_module_t ngx_http_cpp_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_cpp_create_loc_conf, /* create location configuration */
    ngx_http_cpp_merge_loc_conf /* merge location configuration */
};




ngx_module_t ngx_http_cpp_module = {
    NGX_MODULE_V1,
    &ngx_http_cpp_module_ctx, /* module context */
    ngx_http_cpp_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static void * ngx_http_cpp_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_cpp_loc_conf_t *conf = (ngx_http_cpp_loc_conf_t*) ngx_pcalloc(cf->pool, sizeof (ngx_http_cpp_loc_conf_t));
    if (conf) {
        conf->module_dir.len = 0;
        conf->module_dir.data = NULL;
        conf->class_name.len = 0;
        conf->class_name.data = NULL;
        return conf;
    }
    return NGX_CONF_ERROR;
}

static char * ngx_http_cpp_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
    ngx_http_cpp_loc_conf_t * prev = (ngx_http_cpp_loc_conf_t*) parent;
    ngx_http_cpp_loc_conf_t * conf = (ngx_http_cpp_loc_conf_t*) child;

    ngx_conf_merge_str_value(conf->module_dir, prev->module_dir, "");
    ngx_conf_merge_str_value(conf->class_name, prev->class_name, "");

    if (CPP_TOOLS.CLASS_LOADER.isNull())
        CPP_TOOLS.CLASS_LOADER.assign(new Poco::ClassLoader<hi::view>);
    if (CPP_TOOLS.PLUGIN.isNull())
        CPP_TOOLS.PLUGIN.assign(new hi::plugin(std::string((char*) conf->module_dir.data, conf->module_dir.len), CPP_TOOLS.CLASS_LOADER.get()));


    return NGX_CONF_OK;
}

static char *ngx_http_cpp_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *) ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_cpp_handler;
    ngx_conf_set_str_slot(cf, cmd, conf);
    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r) {
    if (r->headers_in.content_length_n > 0) {
        r->request_body_in_file_only = 1;
        r->request_body_in_persistent_file = 1;
        ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_cpp_body_handler);
        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }
        return NGX_DONE;
    } else {
        ngx_http_discard_request_body(r);
        return ngx_http_cpp_normal_handler(r);
    }
}

static ngx_int_t ngx_http_cpp_normal_handler(ngx_http_request_t *r) {
    ngx_http_cpp_loc_conf_t * conf = (ngx_http_cpp_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_cpp_module);
    char* class_name = (char*) conf->class_name.data;
    Poco::SharedPtr<hi::view> view_instance;
    if (conf->class_name.len) {
        auto finded = CPP_TOOLS.CLASS_LOADER->findClass(class_name);
        if (finded && finded->canCreate()) {
            view_instance = CPP_TOOLS.CLASS_LOADER->create(class_name);
        }
    }

    if (view_instance.isNull()) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, ("Failed to allocate view_instance " + std::string(class_name)).c_str());

        return NGX_HTTP_NOT_FOUND;
    }

    hi::request ngx_request;
    hi::response ngx_response;


    get_input_headers(r, ngx_request.headers);
    get_input_cookies(ngx_request.headers, ngx_request.cookies);
    if (r->headers_in.content_length_n > 0) {
        ngx_request.form["temp_body_file_path"] = (char*) (r->request_body->temp_file->file.name.data);
    }
    view_instance->handler(ngx_request, ngx_response);

    ngx_str_t response;
    response.data = (u_char*) ngx_response.content.c_str();
    response.len = ngx_response.content.size();


    ngx_buf_t *buf;
    buf = (ngx_buf_t*) ngx_pcalloc(r->pool, sizeof (ngx_buf_t));
    if (buf == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    buf->pos = response.data;
    buf->last = buf->pos + response.len;
    buf->memory = 1;
    buf->last_buf = 1;

    ngx_chain_t out;
    out.buf = buf;
    out.next = NULL;

    set_output_cookies(ngx_response.cookies, ngx_response.headers, (r->http_connection->ssl ? true : false));
    set_output_headers(r, ngx_response.headers);
    r->headers_out.status = ngx_response.status;
    r->headers_out.content_length_n = response.len;

    ngx_int_t rc;
    rc = ngx_http_send_header(r);
    if (rc != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    return ngx_http_output_filter(r, &out);

}

static void ngx_http_cpp_body_handler(ngx_http_request_t* r) {
    ngx_http_finalize_request(r, ngx_http_cpp_normal_handler(r));
}

static void get_input_headers(ngx_http_request_t* r, std::map<std::string, std::string>& input_headers) {
    ngx_table_elt_t *th;
    ngx_list_part_t *part;
    part = &r->headers_in.headers.part;
    th = (ngx_table_elt_t*) part->elts;
    ngx_uint_t i;
    for (i = 0; /* void */; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            th = (ngx_table_elt_t*) part->elts;
            i = 0;
        }
        input_headers[(char*) th[i].key.data] = (char*) th[i].value.data;
    }
    if (input_headers.find("Connection") != input_headers.end()) {
        r->keepalive = 1;
    }
    input_headers["uri"] = std::string((char*) r->uri.data, r->uri.len).append("?").append((char*) r->args.data, r->args.len);
    input_headers["method"] = std::string((char*) r->method_name.data, r->method_name.len);
    input_headers["client"] = std::string((char*) r->connection->addr_text.data, r->connection->addr_text.len);
}

static void set_output_headers(ngx_http_request_t* r, std::multimap<std::string, std::string>& output_headers) {
    for (auto& item : output_headers) {
        ngx_table_elt_t * h = (ngx_table_elt_t *) ngx_list_push(&r->headers_out.headers);
        if (h) {
            h->hash = 1;
            h->key.data = (u_char*) item.first.c_str();
            h->key.len = item.first.size();
            h->value.data = (u_char*) item.second.c_str();
            h->value.len = item.second.size();
        }
    }

}

static void get_input_cookies(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& cookies) {
    if (input_headers.find("Cookie") != input_headers.end()) {
        const std::string& cookie_header = input_headers.at("Cookie");
        std::string cookie_string = "cookie;" + cookie_header, tmp;
        Poco::Net::NameValueCollection nvc;
        Poco::Net::HTTPMessage::splitParameters(cookie_string, tmp, nvc);
        for (auto & item : nvc) {
            cookies[item.first] = item.second;
        }
    }
}

static void set_output_cookies(const std::map<std::string, std::string>& cookies, std::multimap<std::string, std::string>& output_headers, bool ssl) {
    std::string cookie_header("Set-Cookie");
    for (auto & item : cookies) {
        Poco::Net::HTTPCookie cookie(item.first, item.second);
        cookie.setMaxAge(-1);
        cookie.setPath("/");
        cookie.setHttpOnly(true);
        cookie.setSecure(ssl);
        //cookie.setVersion(1);

        output_headers.insert(std::make_pair(cookie_header, cookie.toString()));
    }
}