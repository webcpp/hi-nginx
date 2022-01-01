#pragma once

#include <string>
#include "servlet.hpp"
#include "kaguya.hpp"

namespace hi
{

    class lua
    {
    public:
        lua();
        lua(const std::string & packsearch,const std::string& cpacksearch,double memory_limit);
        virtual ~lua();

        void main(const request &, response &);
        void init(const std::string & packsearch,const std::string& cpacksearch,double memory_limit);

    private:
        kaguya::State state;
        std::string error_message;
        response *cur_res;
        double memory_limit;
    };

} // namespace hi