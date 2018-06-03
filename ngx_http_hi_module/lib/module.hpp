#ifndef MODULE_HPP
#define MODULE_HPP

#include <memory>
#include <string>
#include <dlfcn.h>

namespace hi {

    template< class T>
    class module {
    public:

        module() : handle(NULL), create(NULL), destroy(NULL), path(), creator(false) {

        }

        module(const std::string& path) : handle(NULL), create(NULL), destroy(NULL), path(path), creator(false) {
            this->init();
        }

        module(const module<T>& other) :
        handle(other.handle), create(other.create), destroy(other.destroy), path(other.path), creator(false) {
        }

        module<T>& operator=(const module<T>& right) {
            if (this == &right)
                return *this;
            this->handle = right.handle;
            this->create = right.create;
            this->destroy = right.destroy;
            this->path = right.path;
            this->creator = false;
            return *this;
        }

        virtual~module() {
            if (this->handle != NULL && this->creator) {
                dlclose(this->handle);
            }
        }

        void set_module(const std::string& path) {
            this->path = path;
            this->init();
        }

        const std::string& get_module()const {
            return this->path;
        }

        template <typename... Args>
        std::shared_ptr<T> make_obj(Args... args) {
            if (this->handle == NULL || this->create == NULL || this->destroy == NULL) {
                return std::shared_ptr<T>(NULL);
            }

            T* obj = this->create(args...);
            destroy_t *d = this->destroy;
            return std::shared_ptr<T>(obj,
                    [&](T * p) {
                        d(p);
                    }
            );
        }
    private:
        typedef T* create_t();
        typedef void destroy_t(T*);
        void* handle;
        create_t *create;
        destroy_t *destroy;
        std::string path;
        bool creator;

        void init() {
            this->handle = dlopen(this->path.c_str(), RTLD_NOW);
            if (this->handle != NULL) {
                dlerror();
                this->create = (create_t*) dlsym(this->handle, "create");
                if (dlerror() != NULL) this->create = NULL;
                this->destroy = (destroy_t*) dlsym(this->handle, "destroy");
                if (dlerror() != NULL)this->destroy = NULL;

                if (this->create != NULL && this->destroy != NULL) {
                    this->creator = true;
                }

            }

        }

    };
}

#endif /* MODULE_HPP */

