#ifndef DUKTAPE_HANDLER_HPP
#define DUKTAPE_HANDLER_HPP

#include "module_config.hpp"
#include "lib/duktape.hpp"

namespace hi {

    static void ngx_http_hi_duktape_handler(ngx_http_request_t *r,ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        hi::py_request py_req;
        hi::py_response py_res;
        py_req.init(&req);
        py_res.init(&res);
        if (!DUKTAPE) {
            DUKTAPE = std::move(std::make_shared<hi::duktape>());
            if (DUKTAPE) {
                DUKTAPE->set_package_path(std::string((char*) conf->duktape_package_path.data, conf->duktape_package_path.len)
                        , std::string((char*) conf->duktape_package_cpath.data, conf->duktape_package_cpath.len));
            }
        }
        if (DUKTAPE) {
            DUKTAPE->set_req(&py_req);
            DUKTAPE->set_res(&py_res);
            if (conf->duktape_script.len > 0) {
                std::string script = std::move(std::string((char*) conf->duktape_script.data, conf->duktape_script.len));
                auto c = script.find_last_of('.');
                if (c == std::string::npos || script.substr(c + 1) != "js") {
                    script.append(req.uri);
                }

                DUKTAPE->call_script(script);

            } else if (conf->duktape_content.len > 0) {

                DUKTAPE->call_content((char*) conf->duktape_content.data);
            }
        }
    }
}

#endif /* DUKTAPE_HANDLER_HPP */

