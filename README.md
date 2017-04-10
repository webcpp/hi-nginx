# Introduction
A distribution of Nginx with c++ web development. 

[hinginx demo](https://github.com/webcpp/hinginx_demo).

# Dependency
- gcc,g++(c++11)
- Poco framework

# Installation
see `init.sh`


# Features
- All features of nginx-1.11.13 are inherited, i.e., it is 100% compatible with nginx.

# Directives
- directives : context :  http, server, location
    - cpp_module_dir,           default ''
    - cpp_route_file,           default ''
    - cpp_upload_dir,           default ''
    - cpp_upload_type,          default 'image/png|image/jpeg|image/gif|application/pdf|application/zip'
    - cpp_upload_field,         default 'upload'
    - cpp_upload_size,          default 10k
    - cpp_session_expire,       default 1h
    - cpp_session_size,         default 1024
    - cpp_cache_size,           default 1024

    example:

```
            cpp_module_dir      /home/ubuntu1610/nginx/cpp;
            cpp_route_file      /home/ubuntu1610/nginx/cpp/route.conf;
            cpp_upload_dir      /home/ubuntu1610/nginx/html/upload;
            cpp_upload_type     image/jpeg|image/png;
            cpp_upload_field    upload|upload1|upload2;
            cpp_upload_size     200k;
            cpp_session_expire  1m;
            cpp_session_size    1024;
            cpp_cache_size      100;
```
- directives : content: location
    - cpp 

    example:
    
```
            location / {
                cpp ;
            }
```

# hello,world

## class

```
#ifndef DEMO_HPP
#define DEMO_HPP

#include "view.hpp"
#include <Poco/Format.h>
#include <Poco/Path.h>
#include <opencv2/opencv.hpp>

namespace nginx {

    class hello : public view {
    public:

        hello() {
        }

        ~hello() {
        }

        void handler(const request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
            std::string key("test");
            if (res.nginx_cache->has(key)) {
                res.content = res.nginx_cache->get(key)->value();
            } else {
                res.content = "hello,world";
                res.nginx_cache->add(key, nginx::cache_string(res.content, 60 * 1000));
            }
            res.status = 200;


        }

    };

    class ses : public view {
    public:

        ses() {
        }

        ~ses() {
        }

        void handler(const request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
            res.content.clear();

            if (res.nginx_session->find("test") == res.nginx_session->end()) {
                res.nginx_session->insert(std::make_pair("test", 1));
            } else {
                int tmp;
                res.nginx_session->at("test").convert<int>(tmp);
                res.nginx_session->at("test") = ++tmp;
            }
            res.cookies["test"] = res.nginx_session->at("test").toString();

            for (auto & item : *res.nginx_session) {
                res.content.append(Poco::format("session:\r\n%[0]s = %[1]s\r\n", item.first, item.second.toString()));
            }
            for (auto & item : req.cookies) {
                res.content.append(Poco::format("cookies:\r\n%[0]s = %[1]s\r\n", item.first, item.second));
            }
            res.status = 200;
        }

    };

    class empty : public view {
    public:

        empty() {
        }

        ~empty() {
        }

        void handler(const request& req, response& res) {

        }

    };

    class form : public view {
    public:

        form() {
        }

        ~form() {
        }

        void handler(const request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";

            res.status = 200;
            res.content.append("head data\r\n");
            for (auto &item : req.headers) {
                res.content.append(Poco::format("%[0]s\t=\t%[1]s\r\n", item.first, item.second));
            }
            res.content.append("form data\r\n");
            for (auto & item : req.form) {
                res.content.append(Poco::format("%[0]s\t=\t%[1]s\r\n", item.first, item.second));
            }
            res.content.append("route data\r\n");
            for (auto & item : req.route) {
                res.content.append(Poco::format("%[0]s\r\n", item));
            }
        }

    };

    class thum : public view {
    public:

        thum() {
        }

        ~thum() {
        }

        void handler(const request& req, response& res) {
            if (req.form.find("upload") != req.form.end()) {
                const std::string& upload_dir = req.headers.at("cpp_upload_dir");
                Poco::Path src_web_path(req.form.at("upload")),
                        src_real_path(upload_dir + src_web_path.toString());
                Poco::Path des_web_path(src_web_path.parent().toString() + src_web_path.getBaseName() + "-thum." + src_web_path.getExtension())
                        , des_real_path(upload_dir + des_web_path.toString());

                cv::Mat src = cv::imread(src_real_path.toString());
                cv::resize(src, src, cv::Size(src.cols / 3, src.rows / 3), 0, 0, CV_INTER_LINEAR);
                if (cv::imwrite(des_real_path.toString(), src)) {
                    res.content = Poco::format("<img src='%[0]s' />", "/upload" + des_web_path.toString());
                } else {
                    res.content = Poco::format("<p>%[0]s</p>", des_real_path.toString());
                }
            } else {
                res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
                res.content = "Not found upload field.";
            }
        }

    };


}


#endif /* DEMO_HPP */


```
## route

```
GET,^/hello/?$,nginx::hello
GET,^/session/?$,nginx::ses
GET,^/empty/?$,nginx::empty
GET,^/form/?(\?((a|b)=[[:alnum:]]+&?)+)?$,nginx::form
POST,^/form/?$,nginx::form
POST,^/thum/?$,nginx::thum

```