#ifndef CACHE_T_HPP
#define CACHE_T_HPP

#include <string>
#include <ctime>

namespace hi {

    class cache_t {
    public:

        cache_t() : status(200), t(time(0)), content_type(), content() {
        }
        ~cache_t() = default;
        int status;
        time_t t;
        std::string content_type, content;

        bool expired(long long expires)const {
            return difftime(time(0), this->t) > expires;
        }
    };
}

#endif /* CACHE_T_HPP */

