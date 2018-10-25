#ifndef LUA_HPP
#define LUA_HPP


#include <string>
#include "kaguya.hpp"
#include "py_request.hpp"
#include "py_response.hpp"


namespace hi {

    class lua {
    public:

        lua(const std::string& package_path, const std::string& package_cpath) : error_message("<p style='text-align:center;margin:100px;'>Server script error</p>")
        , res(0)
        , state() {
            if (!package_path.empty()) {
                this->state("package.path='" + package_path + "'.. package.path");
            }
            if (!package_cpath.empty()) {
                this->state("package.cpath='" + package_cpath + "'.. package.cpath");
            }
            this->state["hi_request"].setClass(
                    kaguya::UserdataMetatable<py_request>()
                    .setConstructors < py_request()>()
                    .addFunction("uri", &hi::py_request::uri)
                    .addFunction("method", &hi::py_request::method)
                    .addFunction("client", &hi::py_request::client)
                    .addFunction("user_agent", &hi::py_request::user_agent)
                    .addFunction("param", &hi::py_request::param)
                    .addFunction("has_header", &hi::py_request::has_header)
                    .addFunction("has_cookie", &hi::py_request::has_cookie)
                    .addFunction("has_form", &hi::py_request::has_form)
                    .addFunction("has_cache", &hi::py_request::has_cache)
                    .addFunction("has_session", &hi::py_request::has_session)
                    .addFunction("get_header", &hi::py_request::get_header)
                    .addFunction("get_cookie", &hi::py_request::get_cookie)
                    .addFunction("get_form", &hi::py_request::get_form)
                    .addFunction("get_session", &hi::py_request::get_session)
                    .addFunction("get_cache", &hi::py_request::get_cache)
                    );
            this->state["hi_response"].setClass(
                    kaguya::UserdataMetatable<py_response>()
                    .addFunction("status", &hi::py_response::status)
                    .addFunction("content", &hi::py_response::content)
                    .addFunction("header", &hi::py_response::header)
                    .addFunction("session", &hi::py_response::session)
                    .addFunction("cache", &hi::py_response::cache)
                    );
        }

        virtual~lua() {
            this->res = 0;
        }

        void set_req(py_request* req) {
            this->state["hi_req"] = req;
        }

        void set_res(py_response* res) {
            this->res = res;
            this->state["hi_res"] = res;
        }

        void call_script(const std::string& lua_script) {
            this->state.setErrorHandler([&](int errCode, const char * szError) {
                this->res->content(szError);
                this->res->status(500);
            });
            this->state.dofile(lua_script);
        }

        void call_content(const std::string& py_content) {
            this->state.setErrorHandler([&](int errCode, const char * szError) {
                this->res->content(szError);
                this->res->status(500);
            });
            this->state.dostring(py_content);
        }





    private:
        std::string error_message;
        py_response * res;
        kaguya::State state;
    };
}

#endif /* LUA_HPP */

