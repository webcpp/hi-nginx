#pragma once

#include <string>
#include <ctime>

namespace hi
{

    class cache_t
    {
    public:
        cache_t() : status(200), t(time(0)), content_type(), content()
        {
        }
        ~cache_t() = default;
        int status;
        time_t t;
        std::string content_type, content;

        bool expired(long long expires) const
        {
            return difftime(time(0), this->t) > expires;
        }
    };
} // namespace hi
