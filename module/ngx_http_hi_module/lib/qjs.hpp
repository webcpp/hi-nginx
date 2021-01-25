#pragma once

extern "C"
{
#include "qjs/bjson.h"
#include "qjs/cutils.h"
#include "qjs/quickjs-libc.h"
#include "qjs/quickjs.h"
}

#include "file_mmap.hpp"
#include "qjs/quickjs-crypto.hpp"
#include "qjs/quickjs-hash.hpp"
#include "qjs/quickjs-hi.hpp"

#include "py_request.hpp"
#include "py_response.hpp"

namespace hi
{
    class qjs
    {
    private:
        JSRuntime *vm;
        JSContext *ctx;
        size_t ctx_called_count, memory_limit, stack_limit, ctx_called_limit;
        file_mmap jsfile_mmap;
        py_request *req;
        py_response *res;

        void config_ctx()
        {
            if (this->ctx)
            {
                JS_AddIntrinsicBigFloat(this->ctx);
                JS_AddIntrinsicBigDecimal(this->ctx);
                JS_AddIntrinsicOperators(this->ctx);
                JS_EnableBignumExt(this->ctx, TRUE);
                js_init_module_std(this->ctx, "std");
                js_init_module_os(this->ctx, "os");
                js_init_module_bjson(this->ctx, "bjson");
                js_init_module_hi(this->ctx, "hi");
                js_init_module_hash(this->ctx, "hash");
                js_init_module_crypto(this->ctx, "crypto");
            }
        }
        void config_vm()
        {
            if (this->vm)
            {
                JS_SetMemoryLimit(this->vm, this->memory_limit);
                JS_SetMaxStackSize(this->vm, this->stack_limit);
                js_std_init_handlers(this->vm);
                JS_SetModuleLoaderFunc(this->vm, NULL, js_module_loader, NULL);
            }
        }

    public:
        qjs(size_t memory_limit = 1048576 * 1024, size_t stack_limit = 1048576 * 8, size_t ctx_called_limit = 10240)
            : vm(0), ctx(0), ctx_called_count(0), memory_limit(memory_limit), stack_limit(stack_limit), ctx_called_limit(ctx_called_limit), jsfile_mmap()
        {
            this->vm = JS_NewRuntime();
            this->config_vm();
            this->ctx = JS_NewContext(this->vm);
            this->config_ctx();
        }
        virtual ~qjs()
        {
            if (this->vm)
            {
                js_std_free_handlers(this->vm);
            }
            if (this->ctx)
            {
                JS_FreeContext(this->ctx);
            }
            if (this->vm)
            {

                JS_FreeRuntime(this->vm);
            }
        }

    public:
        void set_req(py_request *req)
        {
            this->req = req;
        }

        void set_res(py_response *res)
        {
            this->res = res;
        }
        void call_script(const std::string &path)
        {
            if (this->ctx)
            {
                std::pair<const hi::py_request *, hi::py_response *> http_data;
                http_data.first = req;
                http_data.second = res;
                JS_SetContextOpaque(this->ctx, &http_data);

                std::pair<char *, struct stat> ele;
                if (this->jsfile_mmap.get(path, ele))
                {
                    JSValue ret = JS_Eval(this->ctx, ele.first, ele.second.st_size, path.c_str(), JS_EVAL_TYPE_MODULE);
                    if (JS_IsException(ret))
                    {
                        js_std_dump_error(this->ctx);
                        res->status(500);
                        res->content("Internal Server Error");
                    }
                    JS_FreeValue(ctx, ret);
                    js_std_loop(this->ctx);
                    JS_SetContextOpaque(this->ctx, NULL);
                    if (++this->ctx_called_count >= this->ctx_called_limit)
                    {
                        JS_FreeContext(this->ctx);
                        this->ctx = JS_NewContext(this->vm);
                        this->config_ctx();
                        this->ctx_called_count = 0;
                    }
                }
            }
        }
    };
} // namespace hi
