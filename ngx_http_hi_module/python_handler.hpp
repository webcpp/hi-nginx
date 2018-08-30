#ifndef PYTHON_HANDLER_HPP
#define PYTHON_HANDLER_HPP

#include "module_config.hpp"

#include "lib/py_request.hpp"
#include "lib/py_response.hpp"
#include "lib/pybind11_py.hpp"

namespace hi {

    static void ngx_http_hi_python_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        hi::py_request py_req;
        hi::py_response py_res;
        py_req.init(&req);
        py_res.init(&res);
        if (!PYTHON) {
            PYTHON = std::move(std::make_shared<hi::pybind11_py>());
        }
        if (PYTHON) {
            PYTHON->set_req(&py_req);
            PYTHON->set_res(&py_res);
            if (conf->python_script.len > 0) {
                std::string script = std::move(std::string((char*) conf->python_script.data, conf->python_script.len));
                auto c = script.find_last_of('.');
                if (c == std::string::npos || script.substr(c + 1) != "py") {
                    script.append(req.uri);
                }
                if (is_file(script)) {
                    PYTHON->call_script(script);
                }
            } else if (conf->python_content.len > 0) {

                PYTHON->call_content((char*) conf->python_content.data);
            }
        }
    }
}

#endif /* PYTHON_HANDLER_HPP */

