#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>
#include "cache.hpp"
#include "session.hpp"

namespace hi {

    class response {
    public:

        response() :
        status(404)
        , content("<p style='text-align:center;margin:100px;'>404 Not Found</p>")
        , headers()
        , cookies()
        , session(0)
        , cache(0)
        {
            this->headers.insert(std::make_pair("Content-Type", "text/html;charset=UTF-8"));
        }
        virtual~response() = default;
        
        int status;
        std::string content;
        std::multimap<std::string, std::string> headers;
        std::map<std::string, std::string> cookies;
        std::map<std::string, Poco::DynamicAny> *session;
        hi::cache* cache;
    };
}

#endif /* RESPONSE_HPP */

