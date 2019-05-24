#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <unordered_map>

namespace hi {

    class response {
    public:

        response() :
        status(404)
        , content("<p style='text-align:center;margin:100px;'>404 Not Found</p>")
        , headers()
        , session()
        , cache() {
            this->headers.insert(std::move(std::make_pair(std::move("Content-Type"),std::move("text/html;charset=UTF-8"))));
        }
        virtual~response() = default;

        int status;
        std::string content;
        std::unordered_multimap<std::string, std::string> headers;
        std::unordered_map<std::string, std::string> session, cache;
    };
}

#endif /* RESPONSE_HPP */

