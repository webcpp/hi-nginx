#pragma once

#include "module_config.hpp"

namespace hi
{

    static void get_input_headers(ngx_http_request_t *r, std::unordered_map<std::string, std::string> &input_headers)
    {
        ngx_table_elt_t *th;
        ngx_list_part_t *part;
        part = &r->headers_in.headers.part;
        th = (ngx_table_elt_t *)part->elts;
        ngx_uint_t i;
        for (i = 0; /* void */; i++)
        {
            if (i >= part->nelts)
            {
                if (part->next == NULL)
                {

                    break;
                }
                part = part->next;
                th = (ngx_table_elt_t *)part->elts;
                i = 0;
            }
            input_headers[(char *)th[i].key.data] = std::move(std::string((char *)th[i].value.data, th[i].value.len));
        }
    }

    static void set_output_headers(ngx_http_request_t *r, std::unordered_multimap<std::string, std::string> &output_headers)
    {

        for (auto &item : output_headers)
        {
            ngx_table_elt_t *h = (ngx_table_elt_t *)ngx_list_push(&r->headers_out.headers);
            if (h)
            {

                h->hash = 1;
                h->key.data = (u_char *)item.first.c_str();
                h->key.len = item.first.size();
                h->value.data = (u_char *)item.second.c_str();
                h->value.len = item.second.size();
            }
        }
    }

    static std::string get_input_body(ngx_http_request_t *r)
    {
        size_t len;
        ngx_buf_t *buf;
        ngx_chain_t *cl;
        std::string body;

        if (r->request_body == NULL || r->request_body->bufs == NULL)
        {
            return body;
        }

        cl = r->request_body->bufs;

        do
        {
            buf = cl->buf;
            len = buf->last - buf->pos;
            body.append((const char *)buf->pos, len);
            cl = cl->next;
        } while (cl);

        if (r->request_body->temp_file)
        {
            hi::file_mmap fm;
            auto ret = fm.get((char *)r->request_body->temp_file->file.name.data);
            body.append(ret.first, ret.second.st_size);
        }

        return body;
    }

    static void ngx_http_hi_cpp_handler(ngx_http_request_t *r, ngx_http_hi_loc_conf_t *conf, hi::request &req, hi::response &res)
    {
        std::shared_ptr<hi::servlet> instance = std::move(PLUGIN[conf->module_index]->make_obj());
        if (instance)
        {
            instance->handler(req, res);
        }
    }

} // namespace hi
