#pragma once

#include "request.hpp"
#include "response.hpp"

namespace hi
{

    class servlet
    {
    public:
        servlet() = default;
        virtual ~servlet() = default;

        virtual void handler(const request &req, response &res) = 0;
        typedef servlet *create_t();
        typedef void destroy_t(servlet *);
    };
} // namespace anybyte
