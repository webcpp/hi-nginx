#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include <map>
#include <sstream>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include "request.hpp"
#include "upload_handler.hpp"

namespace hi {

    static void set_get_form(const request& req, std::map<std::string, std::string>& form) {
        Poco::Net::HTTPRequest poco_req;
        poco_req.setMethod(req.method);
        poco_req.setURI(req.uri);
        poco_req.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
        Poco::Net::HTMLForm poco_form(poco_req);

        for (auto & item : poco_form) {
            form[item.first] = item.second;
        }
    }

    static void set_post_or_put_form(const request& req, hi::upload_handler* upload_handler, std::map<std::string, std::string>& form) {
        Poco::Net::HTTPRequest poco_req;
        poco_req.setMethod(req.method);
        poco_req.setURI(req.uri);
        poco_req.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
        poco_req.setContentType(req.headers.at("Content-Type"));

        Poco::Net::HTMLForm* poco_form = 0;
        if (req.save_body_in_file) {
            Poco::File temp_file(req.temp_body_file);
            if (temp_file.exists() && temp_file.canRead()) {
                Poco::FileInputStream is(req.temp_body_file);
                if (upload_handler) {
                    poco_form = new Poco::Net::HTMLForm(poco_req, is, *upload_handler);
                    auto result = upload_handler->get_data();
                    for (auto & item : result) {
                        if (item.ok) {
                            form[item.name] = item.webpath;
                        }
                    }
                } else {
                    poco_form = new Poco::Net::HTMLForm(poco_req, is);
                }
                temp_file.remove();
            }
        } else {
            std::istringstream is(req.temp_body_file);
            if (upload_handler) {
                poco_form = new Poco::Net::HTMLForm(poco_req, is, *upload_handler);
                auto result = upload_handler->get_data();
                for (auto & item : result) {
                    if (item.ok) {
                        form[item.name] = item.webpath;
                    }
                }
            } else {
                poco_form = new Poco::Net::HTMLForm(poco_req, is);
            }
        }
        if (poco_form) {
            for (auto & item : *poco_form) {
                form[item.first] = item.second;
            }
            delete poco_form;
        }
    }
}

#endif /* FORM_HPP */

