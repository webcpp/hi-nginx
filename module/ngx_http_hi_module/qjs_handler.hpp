#pragma once

#include "module_config.hpp"

#include "lib/py_request.hpp"
#include "lib/py_response.hpp"

#include "lib/qjs.hpp"

namespace hi
{

    static void ngx_http_hi_qjs_handler(ngx_http_request_t *r, ngx_http_hi_loc_conf_t *conf, hi::request &req, hi::response &res)
    {
        hi::py_request py_req;
        hi::py_response py_res;
        py_req.init(&req);
        py_res.init(&res);
        if (!QJS)
        {
            QJS = std::move(std::make_shared<hi::qjs>(conf->qjs_memory_limit, conf->qjs_stack_limit, conf->qjs_ctx_called_limit));
        }
        if (QJS)
        {
            QJS->set_req(&py_req);
            QJS->set_res(&py_res);
            if (conf->qjs_script.len > 0)
            {
                std::string script = std::move(std::string((char *)conf->qjs_script.data, conf->qjs_script.len));
                auto c = script.find_last_of('.');
                if (c == std::string::npos || script.substr(c + 1) != "js")
                {
                    script.append(req.uri);
                }

                QJS->call_script(script);
            }
        }
    }
} // namespace hi
