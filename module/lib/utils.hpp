#pragma once

extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}
#include <openssl/aes.h>

#include <string>
#include <vector>
#include <unordered_map>

#include "request.hpp"
#include "response.hpp"

#define FORM_MULTIPART_TYPE "multipart/form-data"
#define FORM_MULTIPART_TYPE_LEN (sizeof(FORM_MULTIPART_TYPE) - 1)
#define FORM_URLENCODED_TYPE "application/x-www-form-urlencoded"
#define FORM_URLENCODED_TYPE_LEN (sizeof(FORM_URLENCODED_TYPE) - 1)

namespace hi
{

    std::string md5(const std::string &str);

    std::string sha1(const std::string &str);

    std::string sha256(const std::string &str);

    std::string sha512(const std::string &str);

    void split(const std::string &str, std::vector<std::string> &v, const char *delimiters);

    std::string real_path(const std::string &path);

    class aes
    {
    public:
        aes(const std::string &key);
        virtual ~aes() = default;

    private:
        std::string encode16(const std::string &str);
        std::string decode16(const std::string &cipher);

        void trim(std::string &s);

    public:
        const std::string &encode(const std::string &str);
        const std::string &decode(const std::string &str);

    private:
        std::string key, cipher, plain;
        AES_KEY en_key, de_key;
    };

    std::string random_string(const std::string &s);

    bool is_dir(const std::string &s);

    bool is_file(const std::string &s);

    std::string read_file(const std::string &path);

    bool upload(hi::request &req, const std::string &body, ngx_http_core_loc_conf_t *clcf, ngx_http_request_t *r, const std::string &temp_dir, std::string &err_msg);

    std::string serialize(const std::unordered_map<std::string, std::string> &m);

    void deserialize(const std::string &str, std::unordered_map<std::string, std::string> &m);

    std::string http_time(time_t *t);

    void get_input_headers(ngx_http_request_t *r, std::unordered_map<std::string, std::string> &input_headers);

    void set_output_headers(ngx_http_request_t *r, std::unordered_multimap<std::string, std::string> &output_headers);

    std::string get_input_body(ngx_http_request_t *r);

    ngx_int_t set_output_headers_body(ngx_http_request_t *r, response &res, ngx_int_t expires, const std::string &lru_cache_key);

    ngx_int_t set_output_headers_body_init(ngx_http_request_t *r, request &req, response &res, ngx_int_t expires, std::string &lru_cache_key);

} // namespace hi
