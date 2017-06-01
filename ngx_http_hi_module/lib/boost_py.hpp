#ifndef BOOST_PY_HPP
#define BOOST_PY_HPP

#include <unistd.h>
#include <string>
#include <boost/python.hpp>


#include "py_request.hpp"
#include "py_response.hpp"



namespace hi {

    class boost_py {
    public:

        boost_py() :
        main()
        , dict()
        , res(0)
        , error_message("<p style='text-align:center;margin:100px;'>Server script error</p>") {
            Py_Initialize();
            this->main = boost::python::import("__main__");
            this->dict = this->main.attr("__dict__");
            this->dict["hi_request"] = boost::python::class_<hi::py_request>("hi_request")
                    .def("uri", &hi::py_request::uri)
                    .def("method", &hi::py_request::method)
                    .def("client", &hi::py_request::client)
                    .def("user_agent", &hi::py_request::user_agent)
                    .def("param", &hi::py_request::param)
                    .def("has_header", &hi::py_request::has_header)
                    .def("has_cookie", &hi::py_request::has_cookie)
                    .def("has_form", &hi::py_request::has_form)
                    .def("has_session", &hi::py_request::has_session)
                    .def("get_header", &hi::py_request::get_header)
                    .def("get_cookie", &hi::py_request::get_cookie)
                    .def("get_form", &hi::py_request::get_form)
                    .def("get_session", &hi::py_request::get_session);
            this->dict["hi_response"] = boost::python::class_<hi::py_response>("hi_response")
                    .def("status", &hi::py_response::status)
                    .def("content", &hi::py_response::content)
                    .def("header", &hi::py_response::header)
                    .def("session", &hi::py_response::session);
        }

        virtual~boost_py() {
            Py_Finalize();
            this->res = 0;
        }

        void set_req(py_request* req) {
            this->dict["hi_req"] = boost::python::ptr(req);
        }

        void set_res(py_response* res) {
            this->res = res;
            this->dict["hi_res"] = boost::python::ptr(res);
        }

        void call_script(const std::string& py_script) {
            if (access(py_script.c_str(), F_OK) == 0) {
                try {
                    boost::python::exec_file(py_script.c_str(), this->dict, this->dict);
                } catch (const boost::python::error_already_set&) {
                    this->clear_error();
                    this->res->status(500);
                    this->res->content(this->error_message);
                }
            }
        }

        void call_content(const std::string& py_content) {
            try {
                boost::python::exec(py_content.c_str(), this->dict, this->dict);
            } catch (const boost::python::error_already_set&) {
                this->clear_error();
                this->res->status(500);
                this->res->content(this->error_message);
            }
        }

        void clear_error() {
            PyErr_Clear();
        }
    private:
        boost::python::object main, dict;
        py_response* res;
        std::string error_message;
    };
}

#endif /* BOOST_PY_HPP */

