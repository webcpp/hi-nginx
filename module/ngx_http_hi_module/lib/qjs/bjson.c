#include "cutils.h"
#include "quickjs-libc.h"

static JSValue js_bjson_read(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    uint8_t* buf;
    uint64_t pos, len;
    JSValue obj;
    size_t size;

    if (JS_ToIndex(ctx, &pos, argv[1]))
        return JS_EXCEPTION;
    if (JS_ToIndex(ctx, &len, argv[2]))
        return JS_EXCEPTION;
    buf = JS_GetArrayBuffer(ctx, &size, argv[0]);
    if (!buf)
        return JS_EXCEPTION;
    if (pos + len > size)
        return JS_ThrowRangeError(ctx, "array buffer overflow");
    obj = JS_ReadObject(ctx, buf + pos, len, 0);
    return obj;
}

static JSValue js_bjson_write(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    size_t len;
    uint8_t* buf;
    JSValue array;

    buf = JS_WriteObject(ctx, &len, argv[0], 0);
    if (!buf)
        return JS_EXCEPTION;
    array = JS_NewArrayBufferCopy(ctx, buf, len);
    js_free(ctx, buf);
    return array;
}

static const JSCFunctionListEntry js_bjson_funcs[] = {
    JS_CFUNC_DEF("read", 3, js_bjson_read),
    JS_CFUNC_DEF("write", 1, js_bjson_write),
};

static int js_bjson_init(JSContext* ctx, JSModuleDef* m)
{
    return JS_SetModuleExportList(ctx, m, js_bjson_funcs,
        countof(js_bjson_funcs));
}

JSModuleDef* js_init_module_bjson(JSContext* ctx, const char* module_name)
{
    JSModuleDef* m;
    m = JS_NewCModule(ctx, module_name, js_bjson_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_bjson_funcs, countof(js_bjson_funcs));
    return m;
}