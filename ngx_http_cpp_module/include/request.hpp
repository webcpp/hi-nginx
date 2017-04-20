#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <vector>

namespace hi {

    class request {
    public:

        request() : client(), method(), uri(), temp_body_file(), headers() {
        }
        virtual~request() = default;
        std::string client, method, uri, temp_body_file;
        std::map<std::string, std::string> headers;
    };
}

#endif /* REQUEST_HPP */

