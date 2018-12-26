#ifndef DUKTAPE_HPP
#define DUKTAPE_HPP

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <unistd.h>
#include <functional>
#include <string>
#include "dukglue/duktape.h"
#include "dukglue/duk_print_alert.h"
#include "dukglue/duk_module_duktape.h"
#include "dukglue/duk_console.h"
#include "dukglue/duktape_object.hpp"
#include "dukglue/dukglue.h"
#include "py_request.hpp"
#include "py_response.hpp"
#include "utils.hpp"

#include "file_mmap.hpp"

namespace hi {

    class duktape_tool {
    public:

        duktape_tool() : data(), dl_map(), ctx(0), cpackage_path() {

        }

        virtual~duktape_tool() {
            for (auto& i : this->dl_map) {
                if (i.second.first != NULL) {
                    dlclose(i.second.first);
                }
            }
        }

        std::string read(const std::string& path) {
            std::string text;
            std::pair<char*, struct stat> ele;
            if (this->data.get(path, ele)) {
                text.assign(ele.first, ele.second.st_size);
                return text;
            }
            return text;
        }

        bool require(const std::string& path, const std::string& fun_name) {
            if (this->dl_map.find(fun_name) != this->dl_map.end()) {
                return true;
            }
            void* handler = dlopen((this->cpackage_path + "/" + path + ".so").c_str(), RTLD_NOW);
            if (handler != NULL) {
                dlerror();
                native_fun* fun = (native_fun*) dlsym(handler, fun_name.c_str());
                if (dlerror() == NULL) {
                    duk_push_c_function(this->ctx, *fun, DUK_VARARGS);
                    duk_put_global_string(this->ctx, fun_name.c_str());
                    this->dl_map[fun_name] = std::move(std::make_pair(handler, fun));
                    return true;
                } else {
                    dlclose(handler);
                }
            }
            return false;
        }

        void init(duk_context* ctx) {
            this->ctx = ctx;
        }

        void set_cpackage_path(const std::string& path) {
            this->cpackage_path = path;
        }

        void free(duktape_object* p) {
            delete p;
            dukglue_invalidate_object(this->ctx, p);
        }
    private:
        typedef duk_ret_t native_fun(duk_context *ctx);
        file_mmap data;
        std::unordered_map<std::string, std::pair<void*, native_fun*>> dl_map;
        duk_context* ctx;
        std::string cpackage_path;

    };

    class duktape {
    public:

        duktape() : ctx(0), tool(), res(0), req(0), file_mmap() {
            this->ctx = duk_create_heap_default();
            this->tool.init(this->ctx);
            duk_module_duktape_init(this->ctx);
            duk_print_alert_init(this->ctx, 0 /*flags*/);
            duk_console_init(this->ctx, 0 /*flags*/);

            dukglue_register_constructor<hi::py_request>(this->ctx, "hi_request");
            dukglue_register_method(this->ctx, &hi::py_request::uri, "uri");
            dukglue_register_method(this->ctx, &hi::py_request::method, "method");
            dukglue_register_method(this->ctx, &hi::py_request::client, "client");
            dukglue_register_method(this->ctx, &hi::py_request::user_agent, "user_agent");
            dukglue_register_method(this->ctx, &hi::py_request::param, "param");
            dukglue_register_method(this->ctx, &hi::py_request::has_header, "has_header");
            dukglue_register_method(this->ctx, &hi::py_request::has_cookie, "has_cookie");
            dukglue_register_method(this->ctx, &hi::py_request::has_form, "has_form");
            dukglue_register_method(this->ctx, &hi::py_request::has_cache, "has_cache");
            dukglue_register_method(this->ctx, &hi::py_request::has_session, "has_session");
            dukglue_register_method(this->ctx, &hi::py_request::get_header, "get_header");
            dukglue_register_method(this->ctx, &hi::py_request::get_cookie, "get_cookie");
            dukglue_register_method(this->ctx, &hi::py_request::get_form, "get_form");
            dukglue_register_method(this->ctx, &hi::py_request::get_session, "get_session");
            dukglue_register_method(this->ctx, &hi::py_request::get_cache, "get_cache");

            dukglue_register_constructor<hi::py_response>(this->ctx, "hi_response");
            dukglue_register_method(this->ctx, &hi::py_response::status, "status");
            dukglue_register_method(this->ctx, &hi::py_response::content, "content");
            dukglue_register_method(this->ctx, &hi::py_response::header, "header");
            dukglue_register_method(this->ctx, &hi::py_response::session, "session");
            dukglue_register_method(this->ctx, &hi::py_response::cache, "cache");


            dukglue_register_constructor<duktape_tool>(this->ctx, "hi_tool");
            dukglue_register_method(this->ctx, &duktape_tool::read, "read");
            dukglue_register_method(this->ctx, &duktape_tool::require, "require");
            dukglue_register_method(this->ctx, &duktape_tool::free, "free");


            dukglue_register_global(this->ctx, &this->tool, "hi_module");

        }

        virtual~duktape() {
            if (this->ctx) {
                duk_destroy_heap(this->ctx);
            }
        }

        void call_script(const std::string& path) {
            std::string mmap_key = std::move(hi::md5(path));
            std::unordered_map<std::string, std::pair<char*, struct stat>>::const_iterator iter;
            struct stat st;
            if (stat(path.c_str(), &st) == 0) {
                if (S_ISREG(st.st_mode)) {
                    if ((iter = this->file_mmap.find(mmap_key)) != this->file_mmap.end()) {
                        if (iter->second.second.st_mtime != st.st_mtime) {
                            munmap(iter->second.first, iter->second.second.st_size);
                            this->file_mmap.erase(iter);
                            goto js_read;
                        }
                        duk_peval_lstring_noresult(this->ctx, iter->second.first, iter->second.second.st_size);
                    } else {
js_read:
                        int ffd = open(path.c_str(), O_RDONLY | O_NONBLOCK);
                        if (ffd > 0) {
                            char *mmap_ptr = (char*) mmap(0, st.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, ffd, 0);
                            if (mmap_ptr == MAP_FAILED) {
                                close(ffd);
                                goto js_500;
                            } else {
                                close(ffd);
                                if (madvise(mmap_ptr, st.st_size, MADV_SEQUENTIAL) == 0) {

                                    duk_peval_lstring_noresult(this->ctx, mmap_ptr, st.st_size);
                                    this->file_mmap[mmap_key] = std::move(std::make_pair(mmap_ptr, st));

                                } else {
                                    munmap(mmap_ptr, st.st_size);
                                    goto js_500;
                                }
                            }
                        } else {
js_500:
                            res->status(500);
                            res->content("Internal Server Error");
                        }

                    }
                } else if (S_ISDIR(st.st_mode)) {
                    res->status(403);
                    res->content("Forbidden");
                }
            } else if ((iter = this->file_mmap.find(mmap_key)) != this->file_mmap.end()) {
                munmap(iter->second.first, iter->second.second.st_size);
                this->file_mmap.erase(iter);
            }
        }

        void call_content(const std::string& content) {
            duk_peval_lstring_noresult(this->ctx, content.c_str(), content.size());
        }

        void set_req(py_request* req) {
            this->req = req;
            dukglue_register_global(this->ctx, this->req, "hi_req");

        }

        void set_res(py_response* res) {
            this->res = res;
            dukglue_register_global(this->ctx, this->res, "hi_res");
        }

        void set_package_path(const std::string& package_path, const std::string& cpackage_path) {
            std::string mod_search = "Duktape.modSearch = function (id, require, exports, module) {return hi_module.read('";
            mod_search += package_path;
            mod_search += "/'+id+'.js'); };";
            duk_peval_lstring_noresult(this->ctx, mod_search.c_str(), mod_search.size());
            this->tool.set_cpackage_path(cpackage_path);
        }



    private:
        duk_context* ctx;
        duktape_tool tool;
        py_response * res;
        py_request* req;
        std::unordered_map<std::string, std::pair<char*, struct stat>> file_mmap;
    };
}

#endif /* DUKTAPE_HPP */

