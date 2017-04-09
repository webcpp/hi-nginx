#ifndef CPP_TOOLS_HPP
#define CPP_TOOLS_HPP

#include <string>
#include <vector>
#include <map>
#include <Poco/SharedPtr.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/UUIDGenerator.h>

#include "plugin.hpp"
#include "route.hpp"
#include "../include/cache.hpp"
#include "../include/session.hpp"
#include "../include/view.hpp"


namespace nginx {

    class cpp_tools_t {
    public:

        cpp_tools_t()
        : CLASS_LOADER()
        , PLUGIN()
        , ROUTER()
        , CACHE()
        , SESSION()
//        , HANDLER()
        {
        }
        virtual~cpp_tools_t() = default;

        cpp_tools_t(const cpp_tools_t & other) :
        CLASS_LOADER(other.CLASS_LOADER)
        , PLUGIN(other.PLUGIN)
        , ROUTER(other.ROUTER)
        , CACHE(other.CACHE)
        , SESSION(other.SESSION)
//        , HANDLER(other.HANDLER)
        {
        }

        cpp_tools_t& operator=(const cpp_tools_t & right) {
            if (this == &right)
                return *this;
            this->CLASS_LOADER = right.CLASS_LOADER;
            this->PLUGIN = right.PLUGIN;
            this->ROUTER = right.ROUTER;
            this->CACHE = right.CACHE;
            this->SESSION = right.SESSION;
//            this->HANDLER = right.HANDLER;
            return *this;
        }


        Poco::SharedPtr<Poco::ClassLoader < nginx::view>> CLASS_LOADER;
        Poco::SharedPtr<nginx::plugin> PLUGIN;
        Poco::SharedPtr<nginx::route> ROUTER;
        Poco::SharedPtr<nginx::cache> CACHE;
        Poco::SharedPtr<nginx::session> SESSION;
//        std::map<std::string, Poco::SharedPtr < nginx::view>> HANDLER;
    };

}

#endif /* CPP_TOOLS_HPP */

