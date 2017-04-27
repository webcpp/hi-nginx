#ifndef MODULE_CLASS_H
#define MODULE_CLASS_H

#include <memory>
#include <string>
#include <dlfcn.h>


namespace hi {

    template <class T>
    class module_class {
    public:

        module_class(std::string module_name) : ready(false), module(module_name) {
            this->shared = std::make_shared<shared_obj>();
            this->ready = this->shared->open_module(this->module);
        }

        ~module_class() {
            if (ready)this->shared->close_module();
        }

        template <typename... Args>
        std::shared_ptr<T> make_obj(Args... args) {
            if (!this->ready) {
                return std::shared_ptr<T>(NULL);
            }
            std::shared_ptr<shared_obj> my_shared = this->shared;
            return std::shared_ptr<T>(this->shared->create(args...),
                    [my_shared](T * p) {
                        my_shared->destroy(p);
                    }
            );
        }

        const std::string& get_module()const {
            return this->module;
        }

    private:

        struct shared_obj {
            typename T::create_t *create = NULL;
            typename T::destroy_t *destroy = NULL;
            void * dll_handle = NULL;

            bool open_module(std::string module) {
                {

                    this->dll_handle = dlopen(module.c_str(), RTLD_LAZY);

                    if (!this->dll_handle) {
                        return false;
                    }


                    dlerror();

                    this->create = (typename T::create_t*) dlsym(this->dll_handle, "create");
                    const char * err = dlerror();
                    if (err) {
                        this->close_module();
                        return false;
                    }

                    this->destroy = (typename T::destroy_t*) dlsym(this->dll_handle, "destroy");
                    err = dlerror();
                    if (err) {
                        this->close_module();
                        return false;
                    }

                    return true;
                }
            }

            void close_module() {
                {
                    if (this->dll_handle) {
                        dlclose(this->dll_handle);
                        this->dll_handle = NULL;
                    }
                    if (this->create) create = NULL;
                    if (this->destroy) destroy = NULL;
                }
            }
        };
        bool ready;
        std::string module;
        std::shared_ptr<shared_obj> shared;
    };

}

#endif /* DLCLASS_HPP */
