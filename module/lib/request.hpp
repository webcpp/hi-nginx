#pragma once

#include <string>
#include <unordered_map>

namespace hi
{

    class request
    {
    public:
        request()
            : client(), user_agent(), method(), uri(), param(), headers(), form(), cookies(), session(), cache()
        {
        }
        virtual ~request() = default;
        bool has_header(const std::string &key) const
        {
            return this->headers.find(key) != this->headers.end();
        }

        const std::string &get_header(const std::string &key) const
        {
            return this->headers.at(key);
        }

        bool has_form(const std::string &key) const
        {
            return this->form.find(key) != this->form.end();
        }

        const std::string &get_form(const std::string &key) const
        {
            return this->form.at(key);
        }

        bool has_cookie(const std::string &key) const
        {
            return this->cookies.find(key) != this->cookies.end();
        }

        const std::string &get_cookie(const std::string &key) const
        {
            return this->cookies.at(key);
        }

        bool has_session(const std::string &key) const
        {
            return this->session.find(key) != this->session.end();
        }

        const std::string &get_session(const std::string &key) const
        {
            return this->session.at(key);
        }

        bool has_cache(const std::string &key) const
        {
            return this->cache.find(key) != this->cache.end();
        }

        const std::string &get_cache(const std::string &key) const
        {
            return this->cache.at(key);
        }
        std::string client, user_agent, method, uri, param;
        std::unordered_map<std::string, std::string> headers, form, cookies, session, cache;
    };
} // namespace hi
