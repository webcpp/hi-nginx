#include "quickjs-hi.hpp"
#include "../py_request.hpp"
#include "../py_response.hpp"
#include <iostream>
#include <utility>

int eval_buf(JSContext* ctx, const void* buf, int buf_len,
    const char* filename, int eval_flags)
{
    JSValue val;
    int ret;

    if ((eval_flags & JS_EVAL_TYPE_MASK) == JS_EVAL_TYPE_MODULE) {
        /* for the modules, we compile then run to be able to set
           import.meta */
        val = JS_Eval(ctx, (const char*)buf, buf_len, filename,
            eval_flags | JS_EVAL_FLAG_COMPILE_ONLY);
        if (!JS_IsException(val)) {
            js_module_set_import_meta(ctx, val, TRUE, TRUE);
            val = JS_EvalFunction(ctx, val);
        }
    } else {
        val = JS_Eval(ctx, (const char*)buf, buf_len, filename, eval_flags);
    }
    if (JS_IsException(val)) {
        js_std_dump_error(ctx);
        ret = -1;
    } else {
        ret = 0;
    }
    JS_FreeValue(ctx, val);
    return ret;
}

int eval_file(JSContext* ctx, const char* filename, int module)
{
    uint8_t* buf;
    int ret, eval_flags;
    size_t buf_len;

    buf = js_load_file(ctx, &buf_len, filename);
    if (!buf) {
        perror(filename);
        exit(1);
    }

    if (module < 0) {
        module = (has_suffix(filename, ".mjs") || JS_DetectModule((const char*)buf, buf_len));
    }
    if (module)
        eval_flags = JS_EVAL_TYPE_MODULE;
    else
        eval_flags = JS_EVAL_TYPE_GLOBAL;
    ret = eval_buf(ctx, buf, buf_len, filename, eval_flags);
    js_free(ctx, buf);
    return ret;
}

/**    qjs_server               **/
/**    request **/
static JSValue js_hi_request_get_uri(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    return JS_NewString(ctx, data->first->uri().c_str());
}

static JSValue js_hi_request_get_method(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    return JS_NewString(ctx, data->first->method().c_str());
}

static JSValue js_hi_request_get_client(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    return JS_NewString(ctx, data->first->client().c_str());
}

static JSValue js_hi_request_get_user_agent(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    return JS_NewString(ctx, data->first->user_agent().c_str());
}

static JSValue js_hi_request_get_param(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    return JS_NewString(ctx, data->first->param().c_str());
}

static JSValue js_hi_request_has_header(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewBool(ctx, data->first->has_header(name) ? TRUE : FALSE);
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_has_cookie(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewBool(ctx, data->first->has_cookie(name) ? TRUE : FALSE);
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_has_form(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewBool(ctx, data->first->has_form(name) ? TRUE : FALSE);
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_has_cache(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewBool(ctx, data->first->has_cache(name) ? TRUE : FALSE);
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_has_session(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewBool(ctx, data->first->has_session(name) ? TRUE : FALSE);
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_get_header(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewString(ctx, data->first->get_header(name).c_str());
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_get_cookie(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewString(ctx, data->first->get_cookie(name).c_str());
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_get_form(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewString(ctx, data->first->get_form(name).c_str());
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_get_session(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewString(ctx, data->first->get_session(name).c_str());
    JS_FreeCString(ctx, name);
    return value;
}

static JSValue js_hi_request_get_cache(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_EXCEPTION;
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    JSValue value = JS_NewString(ctx, data->first->get_cache(name).c_str());
    JS_FreeCString(ctx, name);
    return value;
}

/**  response **/

static JSValue js_hi_response_set_header(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 2) {
        return JS_NewBool(ctx, FALSE);
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    const char* value = JS_ToCString(ctx, argv[1]);
    data->second->header(name, value);
    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);
    return JS_NewBool(ctx, TRUE);
}

static JSValue js_hi_response_set_cache(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 2) {
        return JS_NewBool(ctx, FALSE);
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    const char* value = JS_ToCString(ctx, argv[1]);
    data->second->cache(name, value);
    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);
    return JS_NewBool(ctx, TRUE);
}

static JSValue js_hi_response_set_session(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 2) {
        return JS_NewBool(ctx, FALSE);
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* name = JS_ToCString(ctx, argv[0]);
    const char* value = JS_ToCString(ctx, argv[1]);
    data->second->session(name, value);
    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);
    return JS_NewBool(ctx, TRUE);
}

static JSValue js_hi_response_set_status(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    int status;
    if (argc < 1 || JS_ToInt32(ctx, &status, argv[0])) {
        return JS_NewBool(ctx, FALSE);
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    data->second->status(status);
    return JS_NewBool(ctx, TRUE);
}

static JSValue js_hi_response_set_content(JSContext* ctx, JSValueConst this_val,
    int argc, JSValueConst* argv)
{
    if (argc < 1) {
        return JS_NewBool(ctx, FALSE);
    }
    std::pair<const hi::py_request*, hi::py_response*>* data = (std::pair<const hi::py_request*, hi::py_response*>*)JS_GetContextOpaque(ctx);
    const char* content = JS_ToCString(ctx, argv[0]);
    data->second->content(content);
    JS_FreeCString(ctx, content);
    return JS_NewBool(ctx, TRUE);
}

static const JSCFunctionListEntry js_hi_funcs[] = {
    JS_CFUNC_DEF("uri", 0, js_hi_request_get_uri),
    JS_CFUNC_DEF("method", 0, js_hi_request_get_method),
    JS_CFUNC_DEF("client", 0, js_hi_request_get_client),
    JS_CFUNC_DEF("user_agent", 0, js_hi_request_get_user_agent),
    JS_CFUNC_DEF("param", 0, js_hi_request_get_param),
    JS_CFUNC_DEF("has_header", 1, js_hi_request_has_header),
    JS_CFUNC_DEF("has_cache", 1, js_hi_request_has_cache),
    JS_CFUNC_DEF("has_form", 1, js_hi_request_has_form),
    JS_CFUNC_DEF("has_session", 1, js_hi_request_has_session),
    JS_CFUNC_DEF("has_cookie", 1, js_hi_request_has_cookie),
    JS_CFUNC_DEF("get_header", 1, js_hi_request_get_header),
    JS_CFUNC_DEF("get_cache", 1, js_hi_request_get_cache),
    JS_CFUNC_DEF("get_form", 1, js_hi_request_get_form),
    JS_CFUNC_DEF("get_session", 1, js_hi_request_get_session),
    JS_CFUNC_DEF("get_cookie", 1, js_hi_request_get_cookie),
    JS_CFUNC_DEF("status", 1, js_hi_response_set_status),
    JS_CFUNC_DEF("content", 1, js_hi_response_set_content),
    JS_CFUNC_DEF("header", 2, js_hi_response_set_header),
    JS_CFUNC_DEF("cache", 2, js_hi_response_set_cache),
    JS_CFUNC_DEF("session", 2, js_hi_response_set_session),
};

static int js_hi_init(JSContext* ctx, JSModuleDef* m)
{
    return JS_SetModuleExportList(ctx, m, js_hi_funcs,
        countof(js_hi_funcs));
}

JSModuleDef* js_init_module_hi(JSContext* ctx, const char* module_name)
{
    JSModuleDef* m;
    m = JS_NewCModule(ctx, module_name, js_hi_init);
    if (!m)
        return NULL;
    JS_AddModuleExportList(ctx, m, js_hi_funcs, countof(js_hi_funcs));
    return m;
}