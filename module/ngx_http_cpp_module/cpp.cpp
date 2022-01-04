#include <unistd.h>
#include <dlfcn.h>
#include "utils.hpp"
#include "cpp.hpp"

namespace hi
{
    /*
    cpp::cb_t::cb_t(const std::string &path) : m(0), create(0), destroy(0)
    {
        this->m = dlopen(path.c_str(), RTLD_NOW);
        if (this->m)
        {
            this->create = (create_t *)dlsym(m, "create");
            this->destroy = (destroy_t *)dlsym(m, "destroy");
        }
    }
    cpp::cb_t::~cb_t()
    {
        if (this->m)
        {
            dlclose(this->m);
        }
    }

    servlet *cpp::cb_t::make_obj()
    {
        return this->create ? this->create() : NULL;
    }

    void cpp::cb_t::free(servlet *obj)
    {
        this->destroy(obj);
    }

    void cpp::cb_t::main(const request &req, response &res)
    {
        servlet *obj = this->make_obj();
        if (obj)
        {
            obj->handler(req, res);
            this->free(obj);
        }
    }
    */
    cpp::cpp() : path(), modules() {}
    cpp::cpp(const std::string &path) : path(path), modules()
    {
    }
    cpp::~cpp() {}

    void cpp::main(const request &req, response &res)
    {
        std::string key = std::move(hi::md5(req.uri));

        if (this->modules.find(key) == this->modules.end())
        {
            size_t p = req.uri.find_last_of('.');
            std::string m_str;
            if (p != std::string::npos)
            {
                m_str = std::move(this->path + req.uri.substr(0, p).append(".so"));
            }
            else
            {
                m_str = std::move(this->path + req.uri + ".so");
            }

            std::shared_ptr<module<servlet>> cb = std::make_shared<module<servlet>>(m_str);

            if (cb)
            {
                cb->main(req, res);
                this->modules[key] = std::move(cb);
            }
        }
        else
        {

            std::shared_ptr<module<servlet>> cb = this->modules[key];
            cb->main(req, res);
        }
    }

} // namespace anybyte