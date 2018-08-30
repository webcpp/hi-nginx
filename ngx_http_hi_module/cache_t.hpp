#ifndef CACHE_T_HPP
#define CACHE_T_HPP

#include <string>
#include <ctime>

namespace hi {

    struct cache_t {
        int status = 200;
        time_t t;
        std::string content_type, content;

        bool expired(long long expires)const {
            return difftime(time(0), this->t) > expires;
        }
    };
}

#endif /* CACHE_T_HPP */

