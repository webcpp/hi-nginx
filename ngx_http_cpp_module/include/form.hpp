#ifndef FORM_HPP
#define FORM_HPP

#include <string>
#include <map>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include "upload_handler.hpp"

namespace hi {

    static void set_get_form(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& form) {
        Poco::Net::HTTPRequest poco_req;
        poco_req.setMethod(input_headers.at("method"));
        poco_req.setURI(input_headers.at("uri"));
        poco_req.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
        Poco::Net::HTMLForm poco_form(poco_req);

        for (auto & item : poco_form) {
            form[item.first] = item.second;
        }
    }

    static void set_post_or_put_form(const std::string& temp_body_file_path, const std::map<std::string, std::string>& input_headers, hi::upload_handler* upload_handler, std::map<std::string, std::string>& form) {
        Poco::Net::HTTPRequest poco_req;
        poco_req.setMethod(input_headers.at("method"));
        poco_req.setURI(input_headers.at("uri"));
        poco_req.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
        poco_req.setContentType(input_headers.at("Content-Type"));

        Poco::Net::HTMLForm* poco_form = 0;
        Poco::File temp_file(temp_body_file_path);
        if (temp_file.exists() && temp_file.canRead()) {
            Poco::FileInputStream is(temp_body_file_path);
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
        if (poco_form) {
            for (auto & item : *poco_form) {
                form[item.first] = item.second;
            }
            delete poco_form;
        }
    }
}

#endif /* FORM_HPP */

