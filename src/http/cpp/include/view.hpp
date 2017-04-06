#ifndef VIEW_HPP
#define VIEW_HPP

#include "request.hpp"
#include "response.hpp"


namespace nginx {

    class view {
    public:
        view() = default;
        virtual~view() = default;

        virtual void handler(const request& req, response& res) = 0;
    };
}

#endif /* VIEW_HPP */

