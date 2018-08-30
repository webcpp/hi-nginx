#ifndef LUA_HANDLER_HPP
#define LUA_HANDLER_HPP

#include "module_config.hpp"
#include "lib/lua.hpp"

namespace hi {

    static void ngx_http_hi_lua_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        hi::py_request py_req;
        hi::py_response py_res;
        py_req.init(&req);
        py_res.init(&res);
        if (!LUA) {
            LUA = std::move(std::make_shared<hi::lua>(std::string((char*) conf->lua_package_path.data, conf->lua_package_path.len),
                    std::string((char*) conf->lua_package_cpath.data, conf->lua_package_cpath.len)));
        }
        if (LUA) {
            LUA->set_req(&py_req);
            LUA->set_res(&py_res);
            if (conf->lua_script.len > 0) {
                std::string script = std::move(std::string((char*) conf->lua_script.data, conf->lua_script.len));
                auto c = script.find_last_of('.');
                if (c == std::string::npos || script.substr(c + 1) != "lua") {
                    script.append(req.uri);
                }
                if (is_file(script)) {
                    LUA->call_script(script);
                }
            } else if (conf->lua_content.len > 0) {

                LUA->call_content((char*) conf->lua_content.data);
            }
        }
    }
}

#endif /* LUA_HANDLER_HPP */

