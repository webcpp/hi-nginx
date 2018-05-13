#ifndef PYBIND11_PY_HPP
#define PYBIND11_PY_HPP

#include <string>
#include <exception>

#include "py_response.hpp"
#include "py_request.hpp"

#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "pybind11/eval.h"


namespace hi {

    PYBIND11_EMBEDDED_MODULE(pybind11_embed, m) {
        pybind11::class_<hi::py_request>(m, "hi_request")
                .def("uri", &hi::py_request::uri)
                .def("method", &hi::py_request::method)
                .def("client", &hi::py_request::client)
                .def("user_agent", &hi::py_request::user_agent)
                .def("param", &hi::py_request::param)
                .def("has_header", &hi::py_request::has_header)
                .def("has_cookie", &hi::py_request::has_cookie)
                .def("has_form", &hi::py_request::has_form)
                .def("has_session", &hi::py_request::has_session)
                .def("has_cache", &hi::py_request::has_cache)
                .def("get_header", &hi::py_request::get_header)
                .def("get_cookie", &hi::py_request::get_cookie)
                .def("get_form", &hi::py_request::get_form)
                .def("get_session", &hi::py_request::get_session)
                .def("get_cache", &hi::py_request::get_cache);
        pybind11::class_<hi::py_response>(m, "hi_response")
                .def("status", &hi::py_response::status)
                .def("content", &hi::py_response::content)
                .def("header", &hi::py_response::header)
                .def("session", &hi::py_response::session)
                .def("cache", &hi::py_response::cache);
    }

    class pybind11_py {
    public:

        pybind11_py() :
        guard(),
        scope()
        , res(0)
        , error_message("<p style='text-align:center;margin:100px;'>Server script error</p>") {
            this->scope = pybind11::module::import("__main__").attr("__dict__");
            pybind11::module pybind11_py_module = pybind11::module::import("pybind11_embed");
        }

        virtual~pybind11_py() {
            this->res = 0;
        }

        void set_req(py_request* req) {
            this->scope["hi_req"] = *req;
        }

        void set_res(py_response* res) {
            this->res = res;
            this->scope["hi_res"] = *res;
        }

        void call_script(const std::string& py_script) {
            try {
                pybind11::eval_file(py_script.c_str(), this->scope);
            } catch (const std::exception& e) {
                this->clear_error();
                this->res->status(500);
                this->res->content(this->error_message + "<br/><p style='text-align:center;margin:100px;'>" + e.what() + "</p>");
            }
        }

        void call_content(const std::string& py_content) {
            try {
                pybind11::exec(py_content.c_str(), this->scope);
            } catch (const std::exception& e) {
                this->clear_error();
                this->res->status(500);
                this->res->content(this->error_message + "<br/><p>" + e.what() + "</p>");
            }
        }

        void clear_error() {
            PyErr_Clear();
        }
    private:
        pybind11::scoped_interpreter guard;
        pybind11::object scope;
        py_response* res;
        std::string error_message;
    };
}

#endif /* PYBIND11_PY_HPP */

