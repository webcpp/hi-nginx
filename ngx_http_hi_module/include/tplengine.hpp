#ifndef TPLENGINE_HPP
#define TPLENGINE_HPP


#include <sstream>
#include <string>

#include "NLTemplate.hpp"
#include "request.hpp"
#include "response.hpp"

namespace hi {

    class tplengine {
    public:

        tplengine() :
        loader(), tpl(loader) {
        }
        virtual ~tplengine() = default;

        void render(hi::response& res) {
            std::stringbuf buf;
            std::ostream os(&buf);
            this->tpl.render(os);
            res.content = std::move(buf.str());
            res.status = 200;
        }

        tplengine& load(const std::string& path) {
            this->tpl.load(path);
            return *this;
        }

        tplengine& set(const std::string& key, const std::string& value) {
            this->tpl.set(key, value);
            return *this;
        }

        NL::Template::Template& engine() {
            return this->tpl;
        }



    private:
        NL::Template::LoaderFile loader;
        NL::Template::Template tpl;
    };
}

#endif /* TPLENGINE_HPP */

