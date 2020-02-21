#ifndef QUICKJS_CRYPRO_HPP

extern "C" {
#include "cutils.h"
#include "quickjs-libc.h"
}



JSModuleDef *js_init_module_crypto(JSContext *ctx, const char *module_name);



#endif // !QUICKJS_CRYPRO_HPP

