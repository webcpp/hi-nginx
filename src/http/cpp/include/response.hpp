#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include "cache.hpp"
#include "session.hpp"

namespace nginx {

    class response {
    public:

        response() :
        status(200)
        , content("Hello,world")
        , headers()
        , cookies()
        , nginx_session(0)
        , nginx_cache(0)
        {
            this->headers.insert(std::make_pair("Content-Type", "text/html;charset=UTF-8"));
        }
        virtual~response() = default;
        
        int status;
        std::string content;
        std::multimap<std::string, std::string> headers;
        std::map<std::string, std::string> cookies;
        std::map<std::string, Poco::DynamicAny> *nginx_session;
        cache* nginx_cache;
    };
}

#endif /* RESPONSE_HPP */

