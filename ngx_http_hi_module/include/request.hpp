#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>

namespace hi {

    class request {
    public:

        request() : save_body_in_file(false), client(), method(), uri(), temp_body_file(), headers() {
        }
        virtual~request() = default;
        bool save_body_in_file;
        std::string client, method, uri, temp_body_file;
        std::map<std::string, std::string> headers;
    };
}

#endif /* REQUEST_HPP */

