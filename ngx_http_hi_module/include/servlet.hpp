#ifndef SERVLET_HPP
#define SERVLET_HPP

#include "request.hpp"
#include "response.hpp"

#ifdef USE_HIREDIS

#include <memory>
#include "redis.hpp"

#endif

namespace hi {

    class servlet {
    public:
        servlet() = default;
        virtual~servlet() = default;

        virtual void handler(request& req, response& res) = 0;
        typedef servlet * create_t();
        typedef void destroy_t(servlet *);
#ifdef USE_HIREDIS
        std::shared_ptr<redis> REDIS;
#endif
    };
}

#endif /* SERVLET_HPP */

