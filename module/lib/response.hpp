#pragma once

#include <string>
#include <cstdlib>
#include <unordered_map>

namespace hi
{

    class response
    {
    public:
        response()
            : status(404), content("<p style='text-align:center;margin:100px;'>404 Not Found</p>"), headers(), session(), cache()
        {
            this->headers.insert(std::move(std::make_pair(std::move("Content-Type"), std::move("text/html;charset=UTF-8"))));
        }
        virtual ~response() = default;
        void set_header(const std::string &key, const std::string &value)
        {
            if (key == "Content-Type")
            {
                this->headers.find(key)->second = value;
            }
            else
            {
                this->headers.insert(std::move(std::make_pair(key, value)));
            }
        }
        void set_session(const std::string &key, const std::string &value)
        {
            this->session.insert(std::move(std::make_pair(key, value)));
        }
        void set_cache(const std::string &key, const std::string &value)
        {
            this->cache.insert(std::move(std::make_pair(key, value)));
        }
        void set_cookie(const std::string &k, const std::string &v, const std::string &str)
        {
            char buffer[100];
            int ret = snprintf(buffer, 100, "%s=%s; %s", k.c_str(), v.c_str(), str.c_str());
            this->set_header("Set-Cookie", std::string(buffer, ret));
        }
        int status;
        std::string content;
        std::unordered_multimap<std::string, std::string> headers;
        std::unordered_map<std::string, std::string> session, cache;
    };
} // namespace hi
