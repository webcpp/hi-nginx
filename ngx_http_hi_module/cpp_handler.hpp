#ifndef CPP_HANDLER_HPP
#define CPP_HANDLER_HPP

#include "module_config.hpp"

#ifdef HTTP_HI_PYTHON
#include "python_handler.hpp"
#endif

#ifdef HTTP_HI_LUA
#include "lua_handler.hpp"
#endif

#ifdef HTTP_HI_JAVA
#include "java_handler.hpp"
#endif

#ifdef HTTP_HI_PHP
#include "php_handler.hpp"
#endif

namespace hi {

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

    static void ngx_http_hi_cpp_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        std::shared_ptr<hi::servlet> view_instance = std::move(PLUGIN[conf->module_index]->make_obj());
        if (view_instance) {

            view_instance->handler(req, res);
        }

    }




}

#endif /* CPP_HANDLER_HPP */

