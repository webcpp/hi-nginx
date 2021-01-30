#pragma once

#include "module_config.hpp"
#include "lib/java.hpp"

namespace hi
{

    static bool java_init_handler(ngx_http_hi_loc_conf_t *conf)
    {
        if (hi::java::JAVA_IS_READY)
            return hi::java::JAVA_IS_READY;
        if (!JAVA)
        {
            JAVA = std::make_shared<hi::java>((char *)conf->java_classpath.data, (char *)conf->java_options.data, conf->java_version);
            if (JAVA->is_ok())
            {
                JAVA->request = JAVA->env->FindClass("hi/request");
                if (JAVA->request != NULL)
                {
                    JAVA->request_ctor = JAVA->env->GetMethodID(JAVA->request, "<init>", "()V");
                    JAVA->client = JAVA->env->GetFieldID(JAVA->request, "client", "Ljava/lang/String;");
                    JAVA->user_agent = JAVA->env->GetFieldID(JAVA->request, "user_agent", "Ljava/lang/String;");
                    JAVA->method = JAVA->env->GetFieldID(JAVA->request, "method", "Ljava/lang/String;");
                    JAVA->uri = JAVA->env->GetFieldID(JAVA->request, "uri", "Ljava/lang/String;");
                    JAVA->param = JAVA->env->GetFieldID(JAVA->request, "param", "Ljava/lang/String;");
                    JAVA->req_headers = JAVA->env->GetFieldID(JAVA->request, "headers", "Ljava/util/HashMap;");
                    JAVA->form = JAVA->env->GetFieldID(JAVA->request, "form", "Ljava/util/HashMap;");
                    JAVA->cookies = JAVA->env->GetFieldID(JAVA->request, "cookies", "Ljava/util/HashMap;");
                    JAVA->req_session = JAVA->env->GetFieldID(JAVA->request, "session", "Ljava/util/HashMap;");
                    JAVA->req_cache = JAVA->env->GetFieldID(JAVA->request, "cache", "Ljava/util/HashMap;");

                    JAVA->response = JAVA->env->FindClass("hi/response");
                    if (JAVA->response != NULL)
                    {

                        JAVA->response_ctor = JAVA->env->GetMethodID(JAVA->response, "<init>", "()V");
                        JAVA->status = JAVA->env->GetFieldID(JAVA->response, "status", "I");
                        JAVA->content = JAVA->env->GetFieldID(JAVA->response, "content", "Ljava/lang/String;");
                        JAVA->res_headers = JAVA->env->GetFieldID(JAVA->response, "headers", "Ljava/util/HashMap;");
                        JAVA->res_session = JAVA->env->GetFieldID(JAVA->response, "session", "Ljava/util/HashMap;");
                        JAVA->res_cache = JAVA->env->GetFieldID(JAVA->response, "cache", "Ljava/util/HashMap;");

                        JAVA->hashmap = JAVA->env->FindClass("java/util/HashMap");
                        JAVA->hashmap_put = JAVA->env->GetMethodID(JAVA->hashmap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
                        JAVA->hashmap_get = JAVA->env->GetMethodID(JAVA->hashmap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
                        JAVA->hashmap_keyset = JAVA->env->GetMethodID(JAVA->hashmap, "keySet", "()Ljava/util/Set;");

                        JAVA->arraylist = JAVA->env->FindClass("java/util/ArrayList");
                        JAVA->arraylist_get = JAVA->env->GetMethodID(JAVA->arraylist, "get", "(I)Ljava/lang/Object;");
                        JAVA->arraylist_size = JAVA->env->GetMethodID(JAVA->arraylist, "size", "()I");
                        JAVA->arraylist_iterator = JAVA->env->GetMethodID(JAVA->arraylist, "iterator", "()Ljava/util/Iterator;");

                        JAVA->iterator = JAVA->env->FindClass("java/util/Iterator");
                        JAVA->hasnext = JAVA->env->GetMethodID(JAVA->iterator, "hasNext", "()Z");
                        JAVA->next = JAVA->env->GetMethodID(JAVA->iterator, "next", "()Ljava/lang/Object;");

                        JAVA->set = JAVA->env->FindClass("java/util/Set");
                        JAVA->set_iterator = JAVA->env->GetMethodID(JAVA->set, "iterator", "()Ljava/util/Iterator;");

                        JAVA->script_manager = JAVA->env->FindClass("javax/script/ScriptEngineManager");
                        JAVA->script_engine = JAVA->env->FindClass("javax/script/ScriptEngine");

                        JAVA->compilable = JAVA->env->FindClass("javax/script/Compilable");
                        JAVA->compiledscript = JAVA->env->FindClass("javax/script/CompiledScript");

                        JAVA->compile_string = JAVA->env->GetMethodID(JAVA->compilable, "compile", "(Ljava/lang/String;)Ljavax/script/CompiledScript;");
                        JAVA->compile_filereader = JAVA->env->GetMethodID(JAVA->compilable, "compile", "(Ljava/io/Reader;)Ljavax/script/CompiledScript;");
                        JAVA->compiledscript_eval_void = JAVA->env->GetMethodID(JAVA->compiledscript, "eval", "()Ljava/lang/Object;");

                        JAVA->script_manager_ctor = JAVA->env->GetMethodID(JAVA->script_manager, "<init>", "()V");
                        JAVA->script_manager_get_engine_by_name = JAVA->env->GetMethodID(JAVA->script_manager, "getEngineByName", "(Ljava/lang/String;)Ljavax/script/ScriptEngine;");
                        JAVA->script_manager_instance = JAVA->env->NewObject(JAVA->script_manager, JAVA->script_manager_ctor);

                        JAVA->script_engine_put = JAVA->env->GetMethodID(JAVA->script_engine, "put", "(Ljava/lang/String;Ljava/lang/Object;)V");
                        JAVA->script_engine_eval_filereader = JAVA->env->GetMethodID(JAVA->script_engine, "eval", "(Ljava/io/Reader;)Ljava/lang/Object;");
                        JAVA->script_engine_eval_string = JAVA->env->GetMethodID(JAVA->script_engine, "eval", "(Ljava/lang/String;)Ljava/lang/Object;");

                        JAVA->filereader = JAVA->env->FindClass("java/io/FileReader");
                        JAVA->filereader_ctor = JAVA->env->GetMethodID(JAVA->filereader, "<init>", "(Ljava/lang/String;)V");

                        hi::java::JAVA_IS_READY = true;
                    }
                }
            }
        }
        return hi::java::JAVA_IS_READY;
    }

    static void java_input_handler(ngx_http_hi_loc_conf_t *conf, hi::request &req, hi::response &res, jobject request_instance, jobject response_instance)
    {
        jstring client = JAVA->env->NewStringUTF(req.client.c_str());
        JAVA->env->SetObjectField(request_instance, JAVA->client, client);
        JAVA->env->ReleaseStringUTFChars(client, 0);
        JAVA->env->DeleteLocalRef(client);

        jstring user_agent = JAVA->env->NewStringUTF(req.user_agent.c_str());
        JAVA->env->SetObjectField(request_instance, JAVA->user_agent, user_agent);
        JAVA->env->ReleaseStringUTFChars(user_agent, 0);
        JAVA->env->DeleteLocalRef(user_agent);

        jstring method = JAVA->env->NewStringUTF(req.method.c_str());
        JAVA->env->SetObjectField(request_instance, JAVA->method, method);
        JAVA->env->ReleaseStringUTFChars(method, 0);
        JAVA->env->DeleteLocalRef(method);

        jstring uri = JAVA->env->NewStringUTF(req.uri.c_str());
        JAVA->env->SetObjectField(request_instance, JAVA->uri, uri);
        JAVA->env->ReleaseStringUTFChars(uri, 0);
        JAVA->env->DeleteLocalRef(uri);

        jstring param = JAVA->env->NewStringUTF(req.param.c_str());
        JAVA->env->SetObjectField(request_instance, JAVA->param, param);
        JAVA->env->ReleaseStringUTFChars(param, 0);
        JAVA->env->DeleteLocalRef(param);

        if (conf->need_headers == 1)
        {
            jobject req_headers = JAVA->env->GetObjectField(request_instance, JAVA->req_headers);
            for (auto &item : req.headers)
            {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str()), v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_headers, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_headers);
        }

        jobject req_form = JAVA->env->GetObjectField(request_instance, JAVA->form);
        for (auto &item : req.form)
        {
            jstring k = JAVA->env->NewStringUTF(item.first.c_str()), v = JAVA->env->NewStringUTF(item.second.c_str());
            JAVA->env->CallObjectMethod(req_form, JAVA->hashmap_put, k, v);
            JAVA->env->ReleaseStringUTFChars(k, 0);
            JAVA->env->ReleaseStringUTFChars(v, 0);
            JAVA->env->DeleteLocalRef(k);
            JAVA->env->DeleteLocalRef(v);
        }
        JAVA->env->DeleteLocalRef(req_form);

        if (conf->need_cookies == 1)
        {
            jobject req_cookies = JAVA->env->GetObjectField(request_instance, JAVA->cookies);
            for (auto &item : req.cookies)
            {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str()), v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_cookies, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_cookies);
        }

        if (conf->need_session == 1)
        {
            jobject req_session = JAVA->env->GetObjectField(request_instance, JAVA->req_session);
            for (auto &item : req.session)
            {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str()), v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_session, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_session);
        }

        if (conf->need_kvdb == 1)
        {
            jobject req_cache = JAVA->env->GetObjectField(request_instance, JAVA->req_cache);
            for (auto &item : req.cache)
            {

                jstring k = JAVA->env->NewStringUTF(item.first.c_str()), v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_cache, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_cache);
        }
    }

    static void java_output_handler(ngx_http_hi_loc_conf_t *conf, hi::request &req, hi::response &res, jobject request_instance, jobject response_instance)
    {
        jobject res_headers = JAVA->env->GetObjectField(response_instance, JAVA->res_headers);
        jobject keyset = JAVA->env->CallObjectMethod(res_headers, JAVA->hashmap_keyset);
        jobject iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
        while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext))
        {
            jstring k = (jstring)JAVA->env->CallObjectMethod(iterator, JAVA->next);
            jobject item = JAVA->env->CallObjectMethod(res_headers, JAVA->hashmap_get, k);
            jobject jterator = JAVA->env->CallObjectMethod(item, JAVA->arraylist_iterator);
            const char *kstr = JAVA->env->GetStringUTFChars(k, NULL);
            while ((bool)JAVA->env->CallBooleanMethod(jterator, JAVA->hasnext))
            {
                jstring v = (jstring)JAVA->env->CallObjectMethod(jterator, JAVA->next);
                const char *vstr = JAVA->env->GetStringUTFChars(v, NULL);
                if (strcmp(kstr, "Content-Type") == 0)
                {
                    res.headers.find(kstr)->second = vstr;
                }
                else
                {
                    res.headers.insert(std::make_pair(kstr, vstr));
                }

                JAVA->env->ReleaseStringUTFChars(v, vstr);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->ReleaseStringUTFChars(k, kstr);
            JAVA->env->DeleteLocalRef(k);
            JAVA->env->DeleteLocalRef(item);
            JAVA->env->DeleteLocalRef(jterator);
        }
        JAVA->env->DeleteLocalRef(res_headers);
        JAVA->env->DeleteLocalRef(keyset);
        JAVA->env->DeleteLocalRef(iterator);

        if (conf->need_session == 1)
        {
            jobject res_session = JAVA->env->GetObjectField(response_instance, JAVA->res_session);
            keyset = JAVA->env->CallObjectMethod(res_session, JAVA->hashmap_keyset);
            iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
            while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext))
            {
                jstring k = (jstring)JAVA->env->CallObjectMethod(iterator, JAVA->next);
                jstring v = (jstring)JAVA->env->CallObjectMethod(res_session, JAVA->hashmap_get, k);
                const char *kstr = JAVA->env->GetStringUTFChars(k, NULL),
                           *vstr = JAVA->env->GetStringUTFChars(v, NULL);
                res.session[kstr] = vstr;
                JAVA->env->ReleaseStringUTFChars(k, kstr);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->ReleaseStringUTFChars(v, vstr);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(res_session);
            JAVA->env->DeleteLocalRef(keyset);
            JAVA->env->DeleteLocalRef(iterator);
        }

        if (conf->need_kvdb == 1)
        {
            jobject res_cache = JAVA->env->GetObjectField(response_instance, JAVA->res_cache);
            keyset = JAVA->env->CallObjectMethod(res_cache, JAVA->hashmap_keyset);
            iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
            while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext))
            {

                jstring k = (jstring)JAVA->env->CallObjectMethod(iterator, JAVA->next);
                jstring v = (jstring)JAVA->env->CallObjectMethod(res_cache, JAVA->hashmap_get, k);
                const char *kstr = JAVA->env->GetStringUTFChars(k, NULL),
                           *vstr = JAVA->env->GetStringUTFChars(v, NULL);
                res.cache[kstr] = vstr;
                JAVA->env->ReleaseStringUTFChars(k, kstr);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->ReleaseStringUTFChars(v, vstr);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(res_cache);
            JAVA->env->DeleteLocalRef(keyset);
            JAVA->env->DeleteLocalRef(iterator);
        }

        res.status = JAVA->env->GetIntField(response_instance, JAVA->status);
        jstring content = (jstring)JAVA->env->GetObjectField(response_instance, JAVA->content);
        const char *contentstr = JAVA->env->GetStringUTFChars(content, NULL);
        res.content = contentstr;
        JAVA->env->ReleaseStringUTFChars(content, contentstr);
        JAVA->env->DeleteLocalRef(content);
    }

    static void ngx_http_hi_java_handler(ngx_http_request_t *r, ngx_http_hi_loc_conf_t *conf, hi::request &req, hi::response &res)
    {
        if (java_init_handler(conf))
        {

            jobject request_instance, response_instance;

            request_instance = JAVA->env->NewObject(JAVA->request, JAVA->request_ctor);
            response_instance = JAVA->env->NewObject(JAVA->response, JAVA->response_ctor);

            java_input_handler(conf, req, res, request_instance, response_instance);

            hi::java_servlet_t jtmp;
            if (JAVA_SERVLET_CACHE->exists((const char *)conf->java_servlet.data))
            {
                jtmp = JAVA_SERVLET_CACHE->get((const char *)conf->java_servlet.data);
                time_t now = time(0);
                if (difftime(now, jtmp.t) > conf->java_servlet_cache_expires)
                {
                    JAVA_SERVLET_CACHE->erase((const char *)conf->java_servlet.data);
                    goto update_java_servlet;
                }
            }
            else
            {
            update_java_servlet:
                jtmp.SERVLET = JAVA->env->FindClass((const char *)conf->java_servlet.data);

                if (jtmp.SERVLET == NULL)
                    return;
                jtmp.CTOR = JAVA->env->GetMethodID(jtmp.SERVLET, "<init>", "()V");
                jtmp.GET_INSTANCE = JAVA->env->GetStaticMethodID(jtmp.SERVLET, "get_instance", "()Lhi/servlet;");
                jtmp.HANDLER = JAVA->env->GetMethodID(jtmp.SERVLET, "handler", "(Lhi/request;Lhi/response;)V");
                jtmp.t = time(0);
                JAVA_SERVLET_CACHE->put((const char *)conf->java_servlet.data, jtmp);
            }
            jobject servlet_instance;
            if (jtmp.GET_INSTANCE == NULL)
            {
                servlet_instance = JAVA->env->NewObject(jtmp.SERVLET, jtmp.CTOR);
            }
            else
            {
                servlet_instance = JAVA->env->CallStaticObjectMethod(jtmp.SERVLET, jtmp.GET_INSTANCE);
            }
            JAVA->env->CallVoidMethod(servlet_instance, jtmp.HANDLER, request_instance, response_instance);
            JAVA->env->DeleteLocalRef(servlet_instance);

            java_output_handler(conf, req, res, request_instance, response_instance);

            JAVA->env->DeleteLocalRef(request_instance);
            JAVA->env->DeleteLocalRef(response_instance);
        }
    }
} // namespace hi
