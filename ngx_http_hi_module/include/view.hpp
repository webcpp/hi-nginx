#ifndef VIEW_HPP
#define VIEW_HPP

#include "request.hpp"
#include "response.hpp"


namespace hi {

    class view {
    public:
        view() = default;
        virtual~view() = default;

        virtual void handler(request& req, response& res) = 0;
        typedef view * create_t();
        typedef void destroy_t(view *);
    };
}

#endif /* VIEW_HPP */

