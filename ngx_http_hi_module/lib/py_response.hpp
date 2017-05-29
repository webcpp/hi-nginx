#ifndef PY_RESPONSE_HPP
#define PY_RESPONSE_HPP


#include "../include/response.hpp"

namespace hi {

    class py_response {
    public:

        py_response()
        : res(0) {
        }

        virtual~py_response() {
            this->res = 0;
        }

        void init(response* res) {
            this->res = res;
        }

        void status(int c) {
            this->res->status = c;
        }

        void content(const std::string& content) {
            this->res->content = content;
        }

        void header(const std::string& key, const std::string& value) {
            this->res->headers.insert(std::make_pair(key, value));
        }

        void session(const std::string& key, const std::string& value) {
            this->res->session.insert(std::make_pair(key, value));
        }
    private:
        response* res;
    };
}

#endif /* PY_RESPONSE_HPP */

