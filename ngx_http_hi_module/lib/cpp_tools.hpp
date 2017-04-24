#ifndef CPP_TOOLS_HPP
#define CPP_TOOLS_HPP

#include <string>
#include <vector>
#include <map>
#include <Poco/SharedPtr.h>
#include "plugin.hpp"
#include "../include/view.hpp"


namespace hi {

    class cpp_tools_t {
    public:

        cpp_tools_t()
        : CLASS_LOADER()
        , PLUGIN() {
        }
        virtual~cpp_tools_t() = default;

        cpp_tools_t(const cpp_tools_t & other) :
        CLASS_LOADER(other.CLASS_LOADER)
        , PLUGIN(other.PLUGIN) {
        }

        cpp_tools_t& operator=(const cpp_tools_t & right) {
            if (this == &right)
                return *this;
            this->CLASS_LOADER = right.CLASS_LOADER;
            this->PLUGIN = right.PLUGIN;
            return *this;
        }


        Poco::SharedPtr<Poco::ClassLoader< hi::view>> CLASS_LOADER;
        Poco::SharedPtr<hi::plugin> PLUGIN;
    };

}

#endif /* CPP_TOOLS_HPP */

