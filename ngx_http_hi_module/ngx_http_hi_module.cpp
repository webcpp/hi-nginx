extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <vector>

#include "include/request.hpp"
#include "include/response.hpp"
#include "include/view.hpp"
#include "lib/module_class.hpp"


static std::vector<std::shared_ptr<hi::module_class<hi::view>>> PLUGIN;

typedef struct {
    ngx_str_t module_path;
    ngx_int_t module_index;
} ngx_http_hi_loc_conf_t;



static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);
static char *ngx_http_hi_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r);
static void ngx_http_hi_body_handler(ngx_http_request_t* r);
static ngx_int_t ngx_http_hi_normal_handler(ngx_http_request_t *r);


static void get_input_headers(ngx_http_request_t* r, std::map<std::string, std::string>& input_headers);
static void set_output_headers(ngx_http_request_t* r, std::multimap<std::string, std::string>& output_headers);



ngx_command_t ngx_http_hi_commands[] = {
    {
        ngx_string("hi"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_handler,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, module_path),
        NULL
    },

    ngx_null_command
};


ngx_http_module_t ngx_http_hi_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_hi_create_loc_conf, /* create location configuration */
    ngx_http_hi_merge_loc_conf /* merge location configuration */
};




ngx_module_t ngx_http_hi_module = {
    NGX_MODULE_V1,
    &ngx_http_hi_module_ctx, /* module context */
    ngx_http_hi_commands, /* module directives */
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

static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_hi_loc_conf_t *conf = (ngx_http_hi_loc_conf_t*) ngx_pcalloc(cf->pool, sizeof (ngx_http_hi_loc_conf_t));
    if (conf) {
        conf->module_path.len = 0;
        conf->module_path.data = NULL;
        conf->module_index = NGX_CONF_UNSET;
        return conf;
    }
    return NGX_CONF_ERROR;
}

static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
    ngx_http_hi_loc_conf_t * prev = (ngx_http_hi_loc_conf_t*) parent;
    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t*) child;

    ngx_conf_merge_str_value(conf->module_path, prev->module_path, "");
    if (conf->module_index == NGX_CONF_UNSET && conf->module_path.len > 0) {
        std::string tmp((char*) conf->module_path.data, conf->module_path.len);
        if (tmp.front() != '/') {
            tmp.insert(0, NGX_PREFIX);
        }

        ngx_int_t index = NGX_CONF_UNSET;
        bool found = false;
        for (auto& item : PLUGIN) {
            ++index;
            if (item->get_module() == tmp) {
                found = true;
                break;
            }
        }
        if (found) {
            conf->module_index = index;
        } else {
            PLUGIN.push_back(std::make_shared<hi::module_class < hi::view >> (tmp));
            conf->module_index = PLUGIN.size() - 1;
        }
    }

    return NGX_CONF_OK;
}

static char *ngx_http_hi_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *) ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hi_handler;
    ngx_conf_set_str_slot(cf, cmd, conf);
    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r) {
    if (r->headers_in.content_length_n > 0) {
        r->request_body_in_file_only = 1;
        r->request_body_in_persistent_file = 1;
        r->request_body_file_log_level = 0;
        ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_hi_body_handler);
        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }
        return NGX_DONE;
    } else {
        ngx_http_discard_request_body(r);
        return ngx_http_hi_normal_handler(r);
    }
}

static ngx_int_t ngx_http_hi_normal_handler(ngx_http_request_t *r) {
    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_hi_module);
    std::shared_ptr<hi::view> view_instance;
    if (conf->module_index != NGX_CONF_UNSET) {
        view_instance = std::move(PLUGIN[conf->module_index]->make_obj());
    }
    if (!view_instance) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, std::string("Failed to allocate view_instance from ").append((char*) conf->module_path.data).c_str());
        return NGX_HTTP_NOT_IMPLEMENTED;
    }

    hi::request ngx_request;
    hi::response ngx_response;

    get_input_headers(r, ngx_request.headers);
    ngx_request.uri.assign((char*) r->uri.data, r->uri.len).append("?").append((char*) r->args.data, r->args.len);
    ngx_request.method.assign((char*) r->method_name.data, r->method_name.len);
    ngx_request.client.assign((char*) r->connection->addr_text.data, r->connection->addr_text.len);

    if (r->headers_in.content_length_n > 0) {
        ngx_request.temp_body_file.assign((char*) r->request_body->temp_file->file.name.data, r->request_body->temp_file->file.name.len);
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

static void ngx_http_hi_body_handler(ngx_http_request_t* r) {
    ngx_http_finalize_request(r, ngx_http_hi_normal_handler(r));
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