#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>

namespace hi {

    class request {
    public:

        request() : headers(), cookies(), form(), route() {
        }
        virtual~request() = default;
        std::map<std::string, std::string> headers, cookies, form;
        std::vector<std::string> route;
    };
}

#endif /* REQUEST_HPP */

