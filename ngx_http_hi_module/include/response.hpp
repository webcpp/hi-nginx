#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <map>

namespace hi {

    class response {
    public:

        response() :
        status(404)
        , content("<p style='text-align:center;margin:100px;'>404 Not Found</p>")
        , headers(){
            this->headers.insert(std::make_pair("Content-Type", "text/html;charset=UTF-8"));
        }
        virtual~response() = default;

        int status;
        std::string content;
        std::multimap<std::string, std::string> headers;
    };
}

#endif /* RESPONSE_HPP */

