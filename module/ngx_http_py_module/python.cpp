#include <unistd.h>
#include <algorithm>
#include <exception>
#include <functional>

#include "python.hpp"
#include "utils.hpp"
#include "pybind11/eval.h"
#include "pybind11/stl.h"
#include "pybind11/complex.h"
#include "pybind11/functional.h"
#include "pybind11/chrono.h"
#include "pybind11/stl_bind.h"

namespace hi
{
    PYBIND11_EMBEDDED_MODULE(http_py_module, m)
    {
        pybind11::class_<request> request(m, "request");
        request.def(pybind11::init<>())
            .def_readonly("client", &request::client)
            .def_readonly("uri", &request::uri)
            .def_readonly("user_agent", &request::user_agent)
            .def_readonly("method", &request::method)
            .def_readonly("param", &request::param)
            .def_readonly("headers", &request::headers)
            .def_readonly("form", &request::form)
            .def_readonly("cache", &request::cache)
            .def_readonly("cookies", &request::cookies)
            .def_readonly("session", &request::session)
            .def("has_header", &request::has_header)
            .def("has_cache", &request::has_cache)
            .def("has_cookie", &request::has_cookie)
            .def("has_session", &request::has_session)
            .def("has_form", &request::has_form)
            .def("get_header", &request::get_header)
            .def("get_cache", &request::get_cache)
            .def("get_cookie", &request::get_cookie)
            .def("get_session", &request::get_session)
            .def("get_form", &request::get_form);

        pybind11::class_<response> response(m, "response");
        response.def(pybind11::init<>())
            .def_readwrite("status", &response::status)
            .def_readwrite("content", &response::content)
            .def("set_header", &response::set_header)
            .def("set_session", &response::set_session)
            .def("set_cache", &response::set_cache)
            .def("set_cookie", &response::set_cookie);
    }
    python::python() : guard(), os(), sys(), importlib(), anybyte_bind(), error_message("<p style='text-align:center;margin:100px;'>Server script error</p>")
    {
    }
    python::python(const std::string &search_path) : guard(), os(), sys(), importlib(), anybyte_bind(), error_message("<p style='text-align:center;margin:100px;'>Server script error</p>")
    {
        this->init(search_path);
    }

    python::~python()
    {
    }

    void python::init(const std::string &search_path)
    {
        this->sys = pybind11::module::import("sys");
        this->os = pybind11::module::import("os");
        this->importlib = pybind11::module::import("importlib");
        if (!search_path.empty())
        {
            std::vector<std::string> tmp;
            split(search_path, std::ref(tmp), ":");
            auto f = this->sys.attr("path").attr("insert");
            for (const auto &item : tmp)
            {
                f(0, real_path(item).c_str());
            }
        }
        else
        {
            std::string cwd = get_current_dir_name();
            this->sys.attr("path").attr("insert")(0, std::string("\"").append(cwd).append("/app/python\"").c_str());
        }

        this->anybyte_bind = pybind11::module::import("http_py_module");
    }

    void python::main(const request &req, response &res)
    {
        try
        {
            size_t p = req.uri.find_last_of('.');
            std::string m_str;
            if (p != std::string::npos)
            {
                m_str = std::move(req.uri.substr(1, p - 1));
            }
            else
            {
                m_str = std::move(req.uri.substr(1));
            }

            for (std::string::iterator item = m_str.begin(); item != m_str.end(); ++item)
            {
                if (*item == '/')
                {
                    *item = '.';
                }
            }

            pybind11::object handler = this->importlib.attr("import_module")(".index", m_str.c_str()).attr("handler");
            handler(&req, &res);
        }
        catch (const std::exception &e)
        {
            PyErr_Clear();
            res.status = 500;
            res.content = std::move(this->error_message + "<br/><p style='text-align:center;margin:100px;'>" + e.what() + "</p>");
        }
    }

} // namespace anybyte
