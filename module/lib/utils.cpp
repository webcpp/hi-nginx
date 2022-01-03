#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>

#include "request.hpp"
#include "MPFDParser/Parser.h"
#include "jsoncons/json.hpp"
#include "file_mmap.hpp"
#include "cache_t.hpp"
#include "param.hpp"
#include "utils.hpp"

namespace hi
{

    std::string md5(const std::string &str)
    {
        unsigned char digest[MD5_DIGEST_LENGTH] = {0};
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, str.c_str(), str.size());
        MD5_Final(digest, &ctx);

        unsigned char tmp[MD5_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = MD5_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, MD5_DIGEST_LENGTH * 2);
    }

    std::string sha1(const std::string &str)
    {
        unsigned char digest[SHA_DIGEST_LENGTH] = {0};
        SHA_CTX ctx;
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, str.c_str(), str.size());
        SHA1_Final(digest, &ctx);

        unsigned char tmp[SHA_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = SHA_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, SHA_DIGEST_LENGTH * 2);
    }

    std::string sha256(const std::string &str)
    {
        unsigned char digest[SHA256_DIGEST_LENGTH] = {0};
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, str.c_str(), str.size());
        SHA256_Final(digest, &ctx);

        unsigned char tmp[SHA256_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = SHA256_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, SHA256_DIGEST_LENGTH * 2);
    }

    void split(const std::string &str, std::vector<std::string> &v, const char *delimiters)
    {
        char tmp[str.size()];
        strcpy(tmp, str.data());
        char *p = strtok(tmp, delimiters);
        while (p)
        {
            v.push_back(p);
            p = strtok(NULL, delimiters);
        }
    }

    std::string real_path(const std::string &path)
    {
        char path_buffer[PATH_MAX];
        char *tmp = realpath(path.c_str(), path_buffer);
        std::string ret;
        if (tmp)
        {
            ret = tmp;
        }
        return ret;
    }

    aes::aes(const std::string &key)
        : key(key), en_key(), de_key()
    {
        AES_set_encrypt_key((const unsigned char *)this->key.c_str(), 128, &this->en_key);
        AES_set_decrypt_key((const unsigned char *)this->key.c_str(), 128, &this->de_key);
    }

    std::string aes::encode16(const std::string &str)
    {
        unsigned char out[AES_BLOCK_SIZE];
        AES_encrypt((const unsigned char *)str.c_str(), out, &this->en_key);
        return std::string((char *)out, AES_BLOCK_SIZE);
    }
    std::string aes::decode16(const std::string &cipher)
    {
        unsigned char out[AES_BLOCK_SIZE];
        AES_decrypt((const unsigned char *)cipher.c_str(), out, &this->de_key);
        return std::string((char *)out, AES_BLOCK_SIZE);
    }

    void aes::trim(std::string &s)
    {
        if (!s.empty())
        {
            s.erase(0, s.find_first_not_of(" "));
            s.erase(s.find_last_not_of(" ") + 1);
        }
    }

    const std::string &aes::encode(const std::string &str)
    {
        this->cipher.clear();
        size_t len = str.size(), count = len / AES_BLOCK_SIZE, j = 0;
        if (count > 0)
        {
            for (size_t i = 0; i < count; ++i)
            {
                this->cipher.append(this->encode16(str.substr(j, AES_BLOCK_SIZE)));
                j += AES_BLOCK_SIZE;
            }
            this->cipher.append(this->encode16(str.substr(j)));
        }
        else
        {
            this->cipher = this->encode16(str);
        }
        return this->cipher;
    }
    const std::string &aes::decode(const std::string &str)
    {
        this->plain.clear();
        size_t len = str.size(), count = len / AES_BLOCK_SIZE, j = 0;
        if (count > 0)
        {
            for (size_t i = 0; i < count; ++i)
            {
                this->plain.append(this->decode16(str.substr(j, AES_BLOCK_SIZE)));
                j += AES_BLOCK_SIZE;
            }
        }
        else
        {
            this->plain = this->decode16(str);
        }
        this->trim(this->plain);
        return this->plain;
    }

    std::string random_string(const std::string &s)
    {
        time_t now = time(NULL);
        char *now_str = ctime(&now);

        return md5(s + now_str);
    }

    bool is_dir(const std::string &s)
    {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }

    bool is_file(const std::string &s)
    {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISREG(st.st_mode);
    }

    std::string read_file(const std::string &path)
    {
        std::ifstream ifs(path.c_str());
        return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    }

    bool upload(request &req, const std::string &body, ngx_http_core_loc_conf_t *clcf, ngx_http_request_t *r, const std::string &temp_dir, std::string &err_msg)
    {
        bool result = false;
        try
        {
            if ((is_dir(temp_dir) || mkdir(temp_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0))
            {
                std::shared_ptr<MPFD::Parser> POSTParser(new MPFD::Parser());
                POSTParser->SetTempDirForFileUpload(temp_dir);
                POSTParser->SetUploadedFilesStorage(MPFD::Parser::StoreUploadedFilesInFilesystem);
                POSTParser->SetMaxCollectedDataLength(clcf->client_max_body_size);
                POSTParser->SetContentType((char *)r->headers_in.content_type->value.data);
                POSTParser->AcceptSomeData(body.c_str(), body.size());
                auto fields = POSTParser->GetFieldsMap();

                for (auto &item : fields)
                {
                    if (item.second->GetType() == MPFD::Field::TextType)
                    {
                        req.form.insert(std::make_pair(item.first, item.second->GetTextTypeContent()));
                    }
                    else
                    {
                        std::string upload_file_name = item.second->GetFileName(), ext;
                        std::string::size_type p = upload_file_name.find_last_of(".");
                        if (p != std::string::npos)
                        {
                            ext = std::move(upload_file_name.substr(p));
                        }
                        std::string temp_file = std::move(temp_dir + ("/" + random_string(req.client + item.second->GetFileName()).append(ext)));
                        rename(item.second->GetTempFileName().c_str(), temp_file.c_str());
                        req.form.insert(std::make_pair(item.first, temp_file));
                    }
                }
                result = true;
            }
        }
        catch (MPFD::Exception &err)
        {
            err_msg = err.GetError();
        }
        return result;
    }

    std::string serialize(const std::unordered_map<std::string, std::string> &m)
    {
        jsoncons::json j(m);
        return j.as_string();
    }

    void deserialize(const std::string &str, std::unordered_map<std::string, std::string> &m)
    {
        jsoncons::json j = jsoncons::json::parse(str);
        m = std::move(j.as<std::unordered_map<std::string, std::string>>());
    }

    std::string http_time(time_t *t)
    {
        struct tm *timeinfo = gmtime(t);
        char buffer[32] = {0};
        size_t n = strftime(buffer, 32, "%a, %d %b %Y %T GMT", timeinfo);
        return std::string(buffer, n);
    }

    void get_input_headers(ngx_http_request_t *r, std::unordered_map<std::string, std::string> &input_headers)
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

    void set_output_headers(ngx_http_request_t *r, std::unordered_multimap<std::string, std::string> &output_headers)
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

    std::string get_input_body(ngx_http_request_t *r)
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
            file_mmap fm;
            auto ret = fm.get((char *)r->request_body->temp_file->file.name.data);
            body.append(ret.first, ret.second.st_size);
        }

        return body;
    }

    ngx_int_t set_output_headers_body(ngx_http_request_t *r, response &res, ngx_int_t expires, const std::string &lru_cache_key)
    {
        if (expires > 0)
        {
            time_t now = time(0);
            res.headers.insert(std::make_pair("Last-Modified", http_time(&now)));
            std::chrono::system_clock::time_point now_time = std::chrono::system_clock::from_time_t(now);
            std::time_t expire_time = std::chrono::system_clock::to_time_t(now_time + std::chrono::seconds(expires));
            res.headers.insert(std::move(std::make_pair("Expires", http_time(&expire_time))));
            res.headers.insert(std::move(std::make_pair("Age", std::to_string(expires))));
            res.headers.insert(std::move(std::make_pair("Cache-Control", "max-age=" + std::to_string(expires))));

            auto lru_cache_manager = cache_t::get_cache_manager();
            cache_t ele;
            ele.t = now;
            ele.status = res.status;
            ele.content = res.content;
            ele.content_type = res.headers.find("Content-Type")->second;
            lru_cache_manager->insert(lru_cache_key, ele);
        }
        res.headers.insert(std::move(std::make_pair("X-Powered-By", "hi-nginx")));

        ngx_str_t resp;
        resp.data = (u_char *)res.content.c_str();
        resp.len = res.content.size();

        if (resp.len == 0)
        {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to response size.");
            return NGX_HTTP_NOT_FOUND;
        }

        ngx_buf_t *buf;
        buf = (ngx_buf_t *)ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
        if (buf == NULL)
        {
            ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }

        buf->pos = resp.data;
        buf->last = buf->pos + resp.len;
        buf->memory = resp.len ? 1 : 0;
        buf->last_buf = (r == r->main) ? 1 : 0;
        buf->last_in_chain = 1;

        ngx_chain_t out;
        out.buf = buf;
        out.next = NULL;

        set_output_headers(r, res.headers);
        r->headers_out.status = res.status;
        r->headers_out.content_length_n = resp.len;

        ngx_int_t rc;
        rc = ngx_http_send_header(r);
        if (rc != NGX_OK)
        {

            return NGX_HTTP_INTERNAL_SERVER_ERROR;
        }
        return ngx_http_output_filter(r, &out);
    }

    ngx_int_t set_output_headers_body_init(ngx_http_request_t *r, request &req, response &res, ngx_int_t expires, std::string &lru_cache_key)
    {
        req.method.assign((char *)r->method_name.data, r->method_name.len);

        if (expires > 0)
        {
            lru_cache_key = std::move(hi::md5(req.method + req.uri + "?" + req.param));
            auto lru_cache_manager = hi::cache_t::get_cache_manager();
            if (lru_cache_manager->contains(lru_cache_key))
            {
                auto ele = lru_cache_manager->get(lru_cache_key);

                if (ele.expired(expires))
                {
                    lru_cache_manager->remove(lru_cache_key);
                }
                else
                {
                    res.status = ele.status;
                    res.content = ele.content;
                    res.set_header("Content-Type", ele.content_type);
                    return NGX_DONE;
                }
            }
        }

        hi::get_input_headers(r, req.headers);
        req.client.assign((char *)r->connection->addr_text.data, r->connection->addr_text.len);

        if (r->headers_in.user_agent)
        {
            req.user_agent.assign((char *)r->headers_in.user_agent->value.data, r->headers_in.user_agent->value.len);
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
                    return hi::set_output_headers_body(r, res, 0, lru_cache_key);
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
        return NGX_OK;
    }

} // namespace hi