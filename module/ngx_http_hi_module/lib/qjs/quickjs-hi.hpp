#ifndef QUICKJS_MONGOLS_HPP
#define QUICKJS_MONGOLS_HPP

extern "C" {
#include "cutils.h"
#include "quickjs-libc.h"
}

int eval_buf(JSContext* ctx, const void* buf, int buf_len,
    const char* filename, int eval_flags);
int eval_file(JSContext* ctx, const char* filename, int module);

JSModuleDef* js_init_module_hi(JSContext* ctx, const char* module_name);

#endif //