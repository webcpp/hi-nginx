#include "lua.hpp"
#include "utils.hpp"

namespace hi
{

    lua::lua() : state(), error_message("<p style='text-align:center;margin:100px;'>Server script error</p>"), cur_res(0), memory_limit(50.0) {}

    lua::lua(const std::string &packsearch, const std::string &cpacksearch, double memory_limit) : state(), error_message("<p style='text-align:center;margin:100px;'>Server script error</p>"), cur_res(0), memory_limit(50.0)
    {
        this->init(packsearch, cpacksearch, memory_limit);
    }
    lua::~lua() {}

    void lua::init(const std::string &packsearch, const std::string &cpacksearch, double memory_limit)
    {
        this->state["package"]["path"] = this->state["package"]["path"].get<std::string>().append(";" + packsearch);
        this->state["package"]["cpath"] = this->state["package"]["cpath"].get<std::string>().append(";" + cpacksearch);

        this->memory_limit = memory_limit;

        this->state["hi_request"].setClass(
            kaguya::UserdataMetatable<request>()
                .setConstructors<request()>()
                .addProperty("uri", &request::uri)
                .addProperty("client", &request::client)
                .addProperty("method", &request::method)
                .addProperty("user_agent", &request::user_agent)
                .addProperty("param", &request::param)
                .addFunction("has_header", &request::has_header)
                .addFunction("has_cookie", &request::has_cookie)
                .addFunction("has_form", &request::has_form)
                .addFunction("has_cache", &request::has_cache)
                .addFunction("has_session", &request::has_session)
                .addFunction("get_header", &request::get_header)
                .addFunction("get_cookie", &request::get_cookie)
                .addFunction("get_form", &request::get_form)
                .addFunction("get_session", &request::get_session)
                .addFunction("get_cache", &request::get_cache));
        this->state["hi_response"].setClass(
            kaguya::UserdataMetatable<response>()
                .setConstructors<response()>()
                .addProperty("status", &response::status)
                .addProperty("content", &response::content)
                .addFunction("set_header", &response::set_header)
                .addFunction("set_cookie", &response::set_cookie)
                .addFunction("set_session", &response::set_session)
                .addFunction("set_cache", &response::set_cache));
        this->state.setErrorHandler([&](int code, const char *msg)
                                    {
                                        if (this->cur_res)
                                        {
                                            this->cur_res->status = 500;
                                            this->cur_res->content = std::move(this->error_message + "<br/><p style='text-align:center;margin:100px;'>" + std::string(msg) + "</p>");
                                        } });
    }

    void lua::main(const request &req, response &res)
    {
        this->cur_res = &res;

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

        this->state["require"](m_str)["handler"](&req, &res);
        if (this->state["collectgarbage"]("count").get<double>() / 1024.0 > this->memory_limit)
        {
            this->state.garbageCollect();
        }
    }

} // namespace hi