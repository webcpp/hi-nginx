#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <vector>
#include <map>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Net/HTTPMessage.h>

namespace hi {

    static void get_input_cookies(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& cookies) {
        if (input_headers.find("Cookie") != input_headers.end()) {
            const std::string& cookie_header = input_headers.at("Cookie");
            std::string cookie_string = "cookie;" + cookie_header, tmp;
            Poco::Net::NameValueCollection nvc;
            Poco::Net::HTTPMessage::splitParameters(cookie_string, tmp, nvc);
            for (auto & item : nvc) {
                cookies[item.first] = item.second;
            }
        }
    }

    static void set_output_cookies(const std::map<std::string, std::string>& cookies, std::multimap<std::string, std::string>& output_headers, bool ssl = false, bool version = false) {
        std::string cookie_header("Set-Cookie");
        for (auto & item : cookies) {
            Poco::Net::HTTPCookie cookie(item.first, item.second);
            cookie.setMaxAge(-1);
            cookie.setPath("/");
            cookie.setHttpOnly(true);
            cookie.setSecure(ssl);
            cookie.setVersion(version ? 1 : 0);

            output_headers.insert(std::make_pair(cookie_header, cookie.toString()));
        }
    }
}

#endif /* COOKIE_HPP */

