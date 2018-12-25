#ifndef PHP_HANDLER_HPP
#define PHP_HANDLER_HPP

#include "module_config.hpp"
#include "lib/php-x/phpx.h"
#include "lib/php-x/phpx_embed.h"

namespace hi {

    static void ngx_http_hi_php_handler(ngx_http_request_t *r,ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        if (!PHP) {
            int argc = 1;
            char* argv[2] = {"ngx_http_hi_php", NULL};
            PHP = std::move(std::make_shared<php::VM>(argc, argv));
        }
        std::string script = std::move(std::string((char*) conf->php_script.data, conf->php_script.len));
        auto c = script.find_last_of('.');
        if (c == std::string::npos || script.substr(c + 1) != "php") {
            script.append(req.uri);
        }
        if (is_file(script)) {
            zend_first_try{
                PHP->include(script.c_str());
                const char *request = "\\hi\\request", *response = "\\hi\\response", *handler = "handler";
                php::Object php_req = php::newObject(request), php_res = php::newObject(response);
                if (!php_req.isNull() && !php_res.isNull()) {
                    php_req.set("client", php::Variant(req.client));
                    php_req.set("method", php::Variant(req.method));
                    php_req.set("user_agent", php::Variant(req.user_agent));
                    php_req.set("param", php::Variant(req.param));
                    php_req.set("uri", php::Variant(req.uri));

                    php::Array php_req_headers, php_req_form, php_req_cookies, php_req_session, php_req_cache;
                    if (conf->need_headers == 1) {
                        for (auto & i : req.headers) {
                            php_req_headers.set(i.first.c_str(), php::Variant(i.second));
                        }

                    }
                    for (auto & i : req.form) {
                        php_req_form.set(i.first.c_str(), php::Variant(i.second));
                    }
                    if (conf->need_cookies == 1) {
                        for (auto & i : req.cookies) {
                            php_req_cookies.set(i.first.c_str(), php::Variant(i.second));
                        }
                    }
                    if (conf->need_session == 1) {
                        for (auto & i : req.session) {
                            php_req_session.set(i.first.c_str(), php::Variant(i.second));
                        }
                    }
                    if (conf->need_kvdb == 1) {
                        for (auto & i : req.cache) {
                            php_req_cache.set(i.first.c_str(), php::Variant(i.second));
                        }
                    }
                    php_req.set("headers", php_req_headers);
                    php_req.set("form", php_req_form);
                    php_req.set("cookies", php_req_cookies);
                    php_req.set("session", php_req_session);
                    php_req.set("cache", php_req_cache);




                    auto p = script.find_last_of('/'), q = script.find_last_of('.');

                    std::string class_name = std::move(script.substr(p + 1, q - 1 - p));


                    php::Object servlet = php::newObject(class_name.c_str());


                    if (!servlet.isNull() && servlet.methodExists(handler)) {
                        servlet.exec(handler, php_req, php_res);
                        php::Array res_headers = php_res.get("headers"), res_session = php_res.get("session"), res_cache = php_res.get("cache");


                        for (auto i = res_headers.begin(); i != res_headers.end(); i++) {
                            auto v = i.value();
                            if (v.isArray()) {
                                php::Array arr(v);
                                for (size_t j = 0; j < arr.count(); j++) {
                                    res.headers.insert(std::move(std::make_pair(i.key().toString(), arr[j].toString())));
                                }
                                arr.clean();
                            } else {
                                if (i.key().toString() == "Content-Type") {
                                    res.headers.find("Content-Type")->second = std::move(i.value().toString());
                                } else {
                                    res.headers.insert(std::move(std::make_pair(i.key().toString(), i.value().toString())));
                                }
                            }
                        }
                        if (conf->need_session == 1) {
                            for (auto i = res_session.begin(); i != res_session.end(); i++) {
                                res.session.insert(std::move(std::make_pair(i.key().toString(), i.value().toString())));
                            }
                        }
                        if (conf->need_kvdb == 1) {
                            for (auto i = res_cache.begin(); i != res_cache.end(); i++) {
                                res.cache.insert(std::move(std::make_pair(i.key().toString(), i.value().toString())));
                            }
                        }

                        res.content = std::move(php_res.get("content").toString());

                        res.status = php_res.get("status").toInt();


                        php_req_headers.clean();
                        php_req_form.clean();
                        php_req_cookies.clean();
                        php_req_session.clean();

                        res_headers.clean();
                        res_session.clean();

                        return;
                    }
                }}
            zend_catch{
                res.content = std::move("<p style='text-align:center;margin:100px;'>PHP Throw Exception</p>");
                res.status = 500;}
            zend_end_try();
        }
    }
}

#endif /* PHP_HANDLER_HPP */

