#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

namespace hi {

    class request {
    public:

        request() :
        client()
        , user_agent()
        , method()
        , uri()
        , param()
        , headers()
        , form()
        , cookies() {
        }
        virtual~request() = default;
        std::string client, user_agent, method, uri, param;
        std::map<std::string, std::string> headers, form, cookies;
    };
}

#endif /* REQUEST_HPP */

