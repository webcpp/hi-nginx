#include "quickjs-hash.hpp"
#include "../utils.hpp"
static JSValue js_hash_md5(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc != 1) {
        return JS_EXCEPTION;
    }
    const char* content = JS_ToCString(ctx, argv[0]);
    JSValue ret = JS_NewString(ctx, hi::md5(content).c_str());
    JS_FreeCString(ctx, content);
    return ret;
}

static JSValue js_hash_sha1(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc != 1) {
        return JS_EXCEPTION;
    }
    const char* content = JS_ToCString(ctx, argv[0]);
    JSValue ret = JS_NewString(ctx, hi::sha1(content).c_str());
    JS_FreeCString(ctx, content);
    return ret;
}

static JSValue js_hash_sha256(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc != 1) {
        return JS_EXCEPTION;
    }
    const char* content = JS_ToCString(ctx, argv[0]);
    JSValue ret = JS_NewString(ctx, hi::sha256(content).c_str());
    JS_FreeCString(ctx, content);
    return ret;
}

static JSValue js_hash_sha512(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc != 1) {
        return JS_EXCEPTION;
    }
    const char* content = JS_ToCString(ctx, argv[0]);
    JSValue ret = JS_NewString(ctx, hi::sha512(content).c_str());
    JS_FreeCString(ctx, content);
    return ret;
}

static const JSCFunctionListEntry js_hash_funcs[] = {
    JS_CFUNC_DEF("md5", 1, js_hash_md5),
    JS_CFUNC_DEF("sha1", 1, js_hash_sha1),
    JS_CFUNC_DEF("sha256", 1, js_hash_sha256),
    JS_CFUNC_DEF("sha512", 1, js_hash_sha512),
};

static int js_hash_init(JSContext* ctx, JSModuleDef* m)
{
    return JS_SetModuleExportList(ctx, m, js_hash_funcs,
        countof(js_hash_funcs));
}

JSModuleDef* js_init_module_hash(JSContext* ctx, const char* module_name)
{
    JSModuleDef* m;
    m = JS_NewCModule(ctx, module_name, js_hash_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_hash_funcs, countof(js_hash_funcs));
    return m;
}