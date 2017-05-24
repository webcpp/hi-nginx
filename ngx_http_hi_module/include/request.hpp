#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>

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
        , cookies()
        , session() {
        }
        virtual~request() = default;
        std::string client, user_agent, method, uri, param;
        std::unordered_map<std::string, std::string> headers, form, cookies, session;
    };
}

#endif /* REQUEST_HPP */

