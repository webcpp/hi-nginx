#pragma once

#include "servlet.hpp"
#include "module.hpp"
#include <unordered_map>
#include <memory>
#include <string>

namespace hi
{
    class cpp
    {
    public:
        cpp();
        cpp(const std::string &path);

        virtual ~cpp();

        void main(const request &, response &);
        void init(const std::string& path);

    private:
        /*class cb_t
        {
        private:
            typedef servlet *create_t();
            typedef void destroy_t(servlet *);

        public:
            cb_t() = delete;
            cb_t(const std::string &);
            ~cb_t();
            void main(const request &, response &);

        private:
            servlet *make_obj();
            void free(servlet *);

        private:
            void *m;
            create_t *create;
            destroy_t *destroy;
        };*/
        std::string path;
        std::unordered_map<std::string, std::shared_ptr<module<servlet>>> modules;
    };
} //namespace anybyte