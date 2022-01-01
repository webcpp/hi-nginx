#pragma once

#include "servlet.hpp"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"

namespace hi
{

    class python
    {
    public:
        python();
        python(const std::string &search_path);
        virtual ~python();

        void main(const request &, response &);

        void init(const std::string& search_path);

    private:
        pybind11::scoped_interpreter guard;
        pybind11::module os, sys, importlib, anybyte_bind;
        std::string error_message;
    };

} // namespace anybyte