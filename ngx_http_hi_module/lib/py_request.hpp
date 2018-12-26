#ifndef PY_REQUEST_HPP
#define PY_REQUEST_HPP


#include "request.hpp"

namespace hi {

    class py_request {
    public:

        py_request()
        : req(0) {

        }

        virtual~py_request() {
            this->req = 0;
        }

        void init(request* req) {
            this->req = req;
        }

        std::string uri()const {
            return this->req->uri;
        }

        std::string method()const {
            return this->req->method;
        }

        std::string client()const {
            return this->req->client;
        }

        std::string user_agent()const {
            return this->req->user_agent;
        }

        std::string param()const {
            return this->req->param;
        }

        bool has_header(const std::string& key) const {
            return this->req->headers.find(key) != this->req->headers.end();
        }

        std::string get_header(const std::string& key)const {
            return this->req->headers[key];
        }

        bool has_form(const std::string& key) const {
            return this->req->form.find(key) != this->req->form.end();
        }

        std::string get_form(const std::string& key)const {
            return this->req->form[key];
        }

        bool has_cookie(const std::string& key) const {
            return this->req->cookies.find(key) != this->req->cookies.end();
        }

        std::string get_cookie(const std::string& key)const {
            return this->req->cookies[key];
        }

        bool has_session(const std::string& key) const {
            return this->req->session.find(key) != this->req->session.end();
        }

        std::string get_session(const std::string& key)const {
            return this->req->session[key];
        }

        bool has_cache(const std::string& key) const {
            return this->req->cache.find(key) != this->req->cache.end();
        }

        std::string get_cache(const std::string& key)const {
            return this->req->cache[key];
        }

    private:
        request* req;
    };
}


#endif /* PY_REQUEST_HPP */

