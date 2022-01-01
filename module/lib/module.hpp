#pragma once

#include <dlfcn.h>
#include <memory>
#include <string>

namespace hi
{

    template <typename T>
    class module
    {
    private:
        typedef T *create_t();
        typedef void destroy_t(T *);

    public:
        module() = delete;
        module(const std::string &path) : m(0), create(0), destroy(0)
        {
            this->m = dlopen(path.c_str(), RTLD_NOW);
            if (this->m)
            {
                this->create = (create_t *)dlsym(m, "create");
                this->destroy = (destroy_t *)dlsym(m, "destroy");
            }
        }
        ~module()
        {
            if (this->m)
            {
                dlclose(this->m);
            }
        }
        template <typename... ARGS>
        void main(ARGS &&...args)
        {
            T *obj = this->make_obj();
            if (obj)
            {
                obj->handler(args...);
                this->free(obj);
            }
        }

    private:
        T *make_obj()
        {
            return this->create ? this->create() : NULL;
        }
        void free(T *obj)
        {
            this->destroy(obj);
        }

    private:
        void *m;
        create_t *create;
        destroy_t *destroy;
    };
} // namespace hi