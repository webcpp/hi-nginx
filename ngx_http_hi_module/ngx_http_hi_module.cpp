extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>
}

#include <vector>
#include <memory>
#include <future>
#include "include/request.hpp"
#include "include/response.hpp"
#include "include/servlet.hpp"
#include "lib/module_class.hpp"
#include "lib/lrucache.hpp"
#include "lib/param.hpp"


#define form_urlencoded_type "application/x-www-form-urlencoded"
#define form_urlencoded_type_len (sizeof(form_urlencoded_type) - 1)

struct cache_ele_t {
    int status = 200;
    time_t t;
    std::string header, content;
};


static std::vector<std::shared_ptr<hi::module_class<hi::servlet>>> PLUGIN;
static std::vector<std::shared_ptr<cache::lru_cache<std::string, cache_ele_t>>> CACHE;

typedef struct {
    ngx_str_t module_path;
    ngx_int_t module_index;
    ngx_int_t cache_expires;
    ngx_int_t cache_index;
    size_t cache_size;
    ngx_flag_t need_headers;
    ngx_flag_t need_cache;
    ngx_flag_t need_cookies;
} ngx_http_hi_loc_conf_t;


static ngx_int_t clean_up(ngx_conf_t *cf);
static char *ngx_http_hi_conf_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);


static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r);
static void ngx_http_hi_body_handler(ngx_http_request_t* r);
static ngx_int_t ngx_http_hi_normal_handler(ngx_http_request_t *r);


static void get_input_headers(ngx_http_request_t* r, std::unordered_map<std::string, std::string>& input_headers);
static void set_output_headers(ngx_http_request_t* r, std::unordered_multimap<std::string, std::string>& output_headers);
static ngx_str_t get_input_body(ngx_http_request_t *r);


ngx_command_t ngx_http_hi_commands[] = {
    {
        ngx_string("hi"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, module_path),
        NULL
    },
    {
        ngx_string("hi_cache_size"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, cache_size),
        NULL
    },
    {
        ngx_string("hi_cache_expires"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, cache_expires),
        NULL
    },
    {
        ngx_string("hi_need_headers"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_headers),
        NULL
    },
    {
        ngx_string("hi_need_cache"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_cache),
        NULL
    },
    {
        ngx_string("hi_need_cookies"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_cookies),
        NULL
    },
    ngx_null_command
};


ngx_http_module_t ngx_http_hi_module_ctx = {
    clean_up, /* preconfiguration */
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

static ngx_int_t clean_up(ngx_conf_t *cf) {
    PLUGIN.clear();
    CACHE.clear();
    return NGX_OK;
}

static char *ngx_http_hi_conf_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *) ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hi_handler;
    ngx_conf_set_str_slot(cf, cmd, conf);
    return NGX_CONF_OK;
}

static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_hi_loc_conf_t *conf = (ngx_http_hi_loc_conf_t*) ngx_pcalloc(cf->pool, sizeof (ngx_http_hi_loc_conf_t));
    if (conf) {
        conf->module_path.len = 0;
        conf->module_path.data = NULL;
        conf->module_index = NGX_CONF_UNSET;
        conf->cache_size = NGX_CONF_UNSET_UINT;
        conf->cache_expires = NGX_CONF_UNSET;
        conf->cache_index = NGX_CONF_UNSET;
        conf->need_headers = NGX_CONF_UNSET;
        conf->need_cache = NGX_CONF_UNSET;
        conf->need_cookies = NGX_CONF_UNSET;
        return conf;
    }
    return NGX_CONF_ERROR;
}

static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
    ngx_http_hi_loc_conf_t * prev = (ngx_http_hi_loc_conf_t*) parent;
    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t*) child;

    ngx_conf_merge_str_value(conf->module_path, prev->module_path, "");
    ngx_conf_merge_uint_value(conf->cache_size, prev->cache_size, (size_t) 10);
    ngx_conf_merge_sec_value(conf->cache_expires, prev->cache_expires, (ngx_int_t) 300);
    ngx_conf_merge_value(conf->need_headers, prev->need_headers, (ngx_flag_t) 0);
    ngx_conf_merge_value(conf->need_cache, prev->need_cache, (ngx_flag_t) 1);
    ngx_conf_merge_value(conf->need_cookies, prev->need_cookies, (ngx_flag_t) 0);
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
            PLUGIN.push_back(std::make_shared<hi::module_class < hi::servlet >> (tmp));
            conf->module_index = PLUGIN.size() - 1;
        }
    }

    if (conf->cache_index == NGX_CONF_UNSET) {
        CACHE.push_back(std::make_shared<cache::lru_cache < std::string, cache_ele_t >> (conf->cache_size));
        conf->cache_index = CACHE.size() - 1;
    }


    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r) {
    if (r->headers_in.content_length_n > 0) {
        if (r->headers_in.content_type->value.len < form_urlencoded_type_len
                || ngx_strncasecmp(r->headers_in.content_type->value.data, (u_char *) form_urlencoded_type,
                form_urlencoded_type_len) != 0) {
            return NGX_DECLINED;
        }
        r->request_body_in_single_buf = 1;
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
    std::shared_ptr<hi::servlet> view_instance = std::move(PLUGIN[conf->module_index]->make_obj());
    if (!view_instance) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, std::string("Failed to allocate servlet_instance from ").append((char*) conf->module_path.data).c_str());
        return NGX_HTTP_NOT_IMPLEMENTED;
    }

    hi::request ngx_request;
    hi::response ngx_response;

    ngx_request.uri.assign((char*) r->uri.data, r->uri.len);
    if (r->args.len > 0) {
        ngx_request.param.assign((char*) r->args.data, r->args.len);
    }
    std::shared_ptr<std::string> cache_k;
    if (conf->need_cache == 1) {
        cache_k = std::make_shared<std::string>(ngx_request.uri);
        if (r->args.len > 0) {
            cache_k->append("?").append(ngx_request.param);
        }
        u_char *p;
        ngx_md5_t md5;
        u_char md5_buf[16];

        p = (u_char*) ngx_palloc(r->pool, 32);
        if (p == NULL) {
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        ngx_md5_init(&md5);
        ngx_md5_update(&md5, (u_char*) cache_k->c_str(), cache_k->size());
        ngx_md5_final(md5_buf, &md5);
        ngx_hex_dump(p, md5_buf, sizeof (md5_buf));

        cache_k->assign((char*) p, 32);

        if (CACHE[conf->cache_index]->exists(*cache_k)) {
            const cache_ele_t& cache_v = CACHE[conf->cache_index]->get(*cache_k);
            time_t now = time(NULL);
            if (difftime(now, cache_v.t) > conf->cache_expires) {
                CACHE[conf->cache_index]->erase(*cache_k);
            } else {
                ngx_response.content = cache_v.content;
                ngx_response.headers.find("Content-Type")->second = cache_v.header;
                ngx_response.status = cache_v.status;
                goto done;
            }
        }
    }
    if (conf->need_headers == 1) {
        get_input_headers(r, ngx_request.headers);
    }

    ngx_request.method.assign((char*) r->method_name.data, r->method_name.len);
    ngx_request.client.assign((char*) r->connection->addr_text.data, r->connection->addr_text.len);
    if (r->headers_in.user_agent->value.len > 0) {
        ngx_request.user_agent.assign((char*) r->headers_in.user_agent->value.data, r->headers_in.user_agent->value.len);
    }
    if (r->args.len > 0) {
        hi::parser_param(ngx_request.param, ngx_request.form);
    }
    if (r->headers_in.content_length_n > 0) {
        ngx_str_t body = get_input_body(r);
        hi::parser_param(std::string((char*) body.data, body.len), ngx_request.form);
    }
    if (conf->need_cookies == 1 && r->headers_in.cookies.elts != NULL && r->headers_in.cookies.nelts != 0) {
        ngx_table_elt_t ** cookies = (ngx_table_elt_t **) r->headers_in.cookies.elts;
        for (size_t i = 0; i < r->headers_in.cookies.nelts; ++i) {
            if (cookies[i]->value.data != NULL) {
                hi::parser_param(std::string((char*) cookies[i]->value.data, cookies[i]->value.len), ngx_request.cookies, ';');
            }
        }
    }

    view_instance->handler(ngx_request, ngx_response);
    if (conf->need_cache == 1 && conf->cache_expires > 0) {
        cache_ele_t cache_v;
        cache_v.content = ngx_response.content;
        cache_v.header = ngx_response.headers.find("Content-Type")->second;
        cache_v.status = ngx_response.status;
        cache_v.t = time(NULL);
        CACHE[conf->cache_index]->put(*cache_k, cache_v);
    }

done:
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

static void get_input_headers(ngx_http_request_t* r, std::unordered_map<std::string, std::string>& input_headers) {
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
        input_headers[(char*) th[i].key.data] = std::move(std::string((char*) th[i].value.data, th[i].value.len));
    }
}

static void set_output_headers(ngx_http_request_t* r, std::unordered_multimap<std::string, std::string>& output_headers) {
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

static ngx_str_t get_input_body(ngx_http_request_t *r) {
    u_char *p;
    u_char *data;
    size_t len;
    ngx_buf_t *buf, *next;
    ngx_chain_t *cl;
    ngx_str_t body = ngx_null_string;

    if (r->request_body == NULL || r->request_body->bufs == NULL) {
        return body;
    }

    if (r->request_body->temp_file) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "temp_file: %s", r->request_body->temp_file->file.name.data);
        body = r->request_body->temp_file->file.name;
        return body;
    } else {
        cl = r->request_body->bufs;
        buf = cl->buf;

        if (cl->next == NULL) {
            len = buf->last - buf->pos;
            p = (u_char*) ngx_pnalloc(r->pool, len + 1);
            if (p == NULL) {
                return body;
            }
            data = p;
            ngx_memcpy(p, buf->pos, len);
            data[len] = 0;
        } else {
            next = cl->next->buf;
            len = (buf->last - buf->pos) + (next->last - next->pos);
            p = (u_char*) ngx_pnalloc(r->pool, len + 1);
            data = p;
            if (p == NULL) {
                return body;
            }
            p = ngx_cpymem(p, buf->pos, buf->last - buf->pos);
            ngx_memcpy(p, next->pos, next->last - next->pos);
            data[len] = 0;
        }
    }

    body.len = len;
    body.data = data;
    return body;
}
