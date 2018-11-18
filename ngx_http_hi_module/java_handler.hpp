#ifndef JAVA_HANDLER_HPP
#define JAVA_HANDLER_HPP


#include "module_config.hpp"
#include "lib/java.hpp"

namespace hi {

    static bool java_init_handler(ngx_http_hi_loc_conf_t * conf) {
        if (hi::java::JAVA_IS_READY)return hi::java::JAVA_IS_READY;
        if (!JAVA) {
            JAVA = std::make_shared<hi::java>((char*) conf->java_classpath.data, (char*) conf->java_options.data, conf->java_version);
            if (JAVA->is_ok()) {
                JAVA->request = JAVA->env->FindClass("hi/request");
                if (JAVA->request != NULL) {
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
                    if (JAVA->response != NULL) {

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

    static void java_input_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res, jobject request_instance, jobject response_instance) {
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

        if (conf->need_headers == 1) {
            jobject req_headers = JAVA->env->GetObjectField(request_instance, JAVA->req_headers);
            for (auto& item : req.headers) {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str())
                        , v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_headers, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_headers);
        }

        jobject req_form = JAVA->env->GetObjectField(request_instance, JAVA->form);
        for (auto& item : req.form) {
            jstring k = JAVA->env->NewStringUTF(item.first.c_str())
                    , v = JAVA->env->NewStringUTF(item.second.c_str());
            JAVA->env->CallObjectMethod(req_form, JAVA->hashmap_put, k, v);
            JAVA->env->ReleaseStringUTFChars(k, 0);
            JAVA->env->ReleaseStringUTFChars(v, 0);
            JAVA->env->DeleteLocalRef(k);
            JAVA->env->DeleteLocalRef(v);
        }
        JAVA->env->DeleteLocalRef(req_form);

        if (conf->need_cookies == 1) {
            jobject req_cookies = JAVA->env->GetObjectField(request_instance, JAVA->cookies);
            for (auto& item : req.cookies) {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str())
                        , v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_cookies, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_cookies);
        }

        if (conf->need_session == 1) {
            jobject req_session = JAVA->env->GetObjectField(request_instance, JAVA->req_session);
            for (auto& item : req.session) {
                jstring k = JAVA->env->NewStringUTF(item.first.c_str())
                        , v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_session, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_session);
        }

        if (conf->need_kvdb == 1) {
            jobject req_cache = JAVA->env->GetObjectField(request_instance, JAVA->req_cache);
            for (auto& item : req.cache) {

                jstring k = JAVA->env->NewStringUTF(item.first.c_str())
                        , v = JAVA->env->NewStringUTF(item.second.c_str());
                JAVA->env->CallObjectMethod(req_cache, JAVA->hashmap_put, k, v);
                JAVA->env->ReleaseStringUTFChars(k, 0);
                JAVA->env->ReleaseStringUTFChars(v, 0);
                JAVA->env->DeleteLocalRef(k);
                JAVA->env->DeleteLocalRef(v);
            }
            JAVA->env->DeleteLocalRef(req_cache);
        }

    }

    static void java_output_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res, jobject request_instance, jobject response_instance) {
        jobject res_headers = JAVA->env->GetObjectField(response_instance, JAVA->res_headers);
        jobject keyset = JAVA->env->CallObjectMethod(res_headers, JAVA->hashmap_keyset);
        jobject iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
        while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext)) {
            jstring k = (jstring) JAVA->env->CallObjectMethod(iterator, JAVA->next);
            jobject item = JAVA->env->CallObjectMethod(res_headers, JAVA->hashmap_get, k);
            jobject jterator = JAVA->env->CallObjectMethod(item, JAVA->arraylist_iterator);
            const char * kstr = JAVA->env->GetStringUTFChars(k, NULL);
            while ((bool)JAVA->env->CallBooleanMethod(jterator, JAVA->hasnext)) {
                jstring v = (jstring) JAVA->env->CallObjectMethod(jterator, JAVA->next);
                const char* vstr = JAVA->env->GetStringUTFChars(v, NULL);
                res.headers.insert(std::make_pair(kstr, vstr));
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

        if (conf->need_session == 1) {
            jobject res_session = JAVA->env->GetObjectField(response_instance, JAVA->res_session);
            keyset = JAVA->env->CallObjectMethod(res_session, JAVA->hashmap_keyset);
            iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
            while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext)) {
                jstring k = (jstring) JAVA->env->CallObjectMethod(iterator, JAVA->next);
                jstring v = (jstring) JAVA->env->CallObjectMethod(res_session, JAVA->hashmap_get, k);
                const char * kstr = JAVA->env->GetStringUTFChars(k, NULL),
                        * vstr = JAVA->env->GetStringUTFChars(v, NULL);
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

        if (conf->need_kvdb == 1) {
            jobject res_cache = JAVA->env->GetObjectField(response_instance, JAVA->res_cache);
            keyset = JAVA->env->CallObjectMethod(res_cache, JAVA->hashmap_keyset);
            iterator = JAVA->env->CallObjectMethod(keyset, JAVA->set_iterator);
            while ((bool)JAVA->env->CallBooleanMethod(iterator, JAVA->hasnext)) {

                jstring k = (jstring) JAVA->env->CallObjectMethod(iterator, JAVA->next);
                jstring v = (jstring) JAVA->env->CallObjectMethod(res_cache, JAVA->hashmap_get, k);
                const char * kstr = JAVA->env->GetStringUTFChars(k, NULL),
                        * vstr = JAVA->env->GetStringUTFChars(v, NULL);
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
        jstring content = (jstring) JAVA->env->GetObjectField(response_instance, JAVA->content);
        const char* contentstr = JAVA->env->GetStringUTFChars(content, NULL);
        res.content = contentstr;
        JAVA->env->ReleaseStringUTFChars(content, contentstr);
        JAVA->env->DeleteLocalRef(content);
    }

    static bool javascript_engine_init_handler(ngx_http_hi_loc_conf_t * conf) {
        bool result = false;
        if (conf->javascript_engine_index == NGX_CONF_UNSET) {
            std::pair<jobject, jobject> engine{NULL, NULL};
            jstring engine_name = JAVA->env->NewStringUTF((char*) conf->javascript_lang.data);
            engine.first = (jobject) JAVA->env->CallObjectMethod(JAVA->script_manager_instance, JAVA->script_manager_get_engine_by_name, engine_name);
            JAVA->env->ReleaseStringUTFChars(engine_name, 0);
            JAVA->env->DeleteLocalRef(engine_name);
            if (engine.first != NULL) {
                if (JAVA->env->IsInstanceOf(engine.first, JAVA->compilable) == JNI_TRUE) {
                    engine.second = (jobject) engine.first;
                } else {
                    engine.second = NULL;
                }
                JAVA->engines.push_back(engine);
                conf->javascript_engine_index = JAVA->engines.size() - 1;
                result = true;
            }
        } else {

            result = true;
        }
        return result;
    }

    static void ngx_http_hi_java_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        if (java_init_handler(conf)) {

            jobject request_instance, response_instance;


            request_instance = JAVA->env->NewObject(JAVA->request, JAVA->request_ctor);
            response_instance = JAVA->env->NewObject(JAVA->response, JAVA->response_ctor);

            java_input_handler(conf, req, res, request_instance, response_instance);



            hi::java_servlet_t jtmp;
            if (JAVA_SERVLET_CACHE->exists((const char*) conf->java_servlet.data)) {
                jtmp = JAVA_SERVLET_CACHE->get((const char*) conf->java_servlet.data);
                time_t now = time(0);
                if (difftime(now, jtmp.t) > conf->java_servlet_cache_expires) {
                    JAVA_SERVLET_CACHE->erase((const char*) conf->java_servlet.data);
                    goto update_java_servlet;
                }
            } else {
update_java_servlet:
                jtmp.SERVLET = JAVA->env->FindClass((const char*) conf->java_servlet.data);

                if (jtmp.SERVLET == NULL)return;
                jtmp.CTOR = JAVA->env->GetMethodID(jtmp.SERVLET, "<init>", "()V");
                jtmp.HANDLER = JAVA->env->GetMethodID(jtmp.SERVLET, "handler", "(Lhi/request;Lhi/response;)V");
                jtmp.t = time(0);
                JAVA_SERVLET_CACHE->put((const char*) conf->java_servlet.data, jtmp);
            }
            jobject servlet_instance = JAVA->env->NewObject(jtmp.SERVLET, jtmp.CTOR);
            JAVA->env->CallVoidMethod(servlet_instance, jtmp.HANDLER, request_instance, response_instance);
            JAVA->env->DeleteLocalRef(servlet_instance);


            java_output_handler(conf, req, res, request_instance, response_instance);


            JAVA->env->DeleteLocalRef(request_instance);
            JAVA->env->DeleteLocalRef(response_instance);
        }
    }

    static void ngx_http_hi_javascript_handler(ngx_http_hi_loc_conf_t * conf, hi::request& req, hi::response& res) {
        if (java_init_handler(conf) && javascript_engine_init_handler(conf)) {

            std::pair<jobject, jobject>& engine = JAVA->engines[conf->javascript_engine_index];

            if (engine.first == NULL) {
                return;
            }

            jobject request_instance, response_instance;

            request_instance = JAVA->env->NewObject(JAVA->request, JAVA->request_ctor);
            response_instance = JAVA->env->NewObject(JAVA->response, JAVA->response_ctor);

            java_input_handler(conf, req, res, request_instance, response_instance);

            jstring jhi_req = JAVA->env->NewStringUTF("hi_req");
            jstring jhi_res = JAVA->env->NewStringUTF("hi_res");


            JAVA->env->CallVoidMethod(engine.first, JAVA->script_engine_put, jhi_req, request_instance);
            JAVA->env->CallVoidMethod(engine.first, JAVA->script_engine_put, jhi_res, response_instance);

            if (conf->javascript_script.len > 0) {
                std::string script_path = std::move(std::string((char*) conf->javascript_script.data, conf->javascript_script.len));
                auto c = script_path.find_last_of('.');
                if (c == std::string::npos || script_path.substr(c + 1) != (char*) conf->javascript_extension.data) {
                    script_path.append(req.uri);
                }

                if (is_file(script_path)) {
                    std::string md5key = std::move(md5(script_path));
                    if (conf->need_kvdb == 1) {
                        if (JAVA->compiledscript_instances.find(md5key) != JAVA->compiledscript_instances.end()) {
                            time_t now(0);
                            const std::pair<time_t, jobject>& compiledscript_pair = JAVA->compiledscript_instances[md5key];
                            if (difftime(now, compiledscript_pair.first) > conf->javascript_compiledscript_expires) {
                                JAVA->env->DeleteLocalRef(compiledscript_pair.second);
                                JAVA->compiledscript_instances.erase(md5key);
                                goto update_string_compiledscript_instance;
                            }
                            if (compiledscript_pair.second != NULL) {
                                JAVA->env->CallObjectMethod(compiledscript_pair.second, JAVA->compiledscript_eval_void);
                            }
                        } else {
update_string_compiledscript_instance:
                            jstring script_content = NULL;
                            if (LEVELDB) {
                                std::string md5val;
                                if (LEVELDB->Get(leveldb::ReadOptions(), md5key, &md5val).ok()) {
                                    script_content = JAVA->env->NewStringUTF(md5val.c_str());
                                } else {
update_javascript_content:
                                    std::string script_str = std::move(read_file(script_path));
                                    script_content = JAVA->env->NewStringUTF(script_str.c_str());
                                    LEVELDB->Put(leveldb::WriteOptions(), md5key, script_str);
                                }
                            } else {
                                script_content = JAVA->env->NewStringUTF(read_file(script_path).c_str());
                            }
                            if (engine.second != NULL) {
                                jobject compiledscript_instance = (jobject) JAVA->env->CallObjectMethod(engine.second, JAVA->compile_string, script_content);
                                if (compiledscript_instance != NULL) {
                                    JAVA->compiledscript_instances[md5key] = std::make_pair(time(0), compiledscript_instance);
                                    JAVA->env->CallObjectMethod(compiledscript_instance, JAVA->compiledscript_eval_void);
                                }
                            } else {
                                JAVA->env->CallObjectMethod(engine.first, JAVA->script_engine_eval_string, script_content);
                            }

                            if (script_content != NULL) {
                                JAVA->env->ReleaseStringUTFChars(script_content, 0);
                                JAVA->env->DeleteLocalRef(script_content);
                            }
                        }

                    } else {
                        if (JAVA->compiledscript_instances.find(md5key) != JAVA->compiledscript_instances.end()) {
                            time_t now(NULL);
                            const std::pair<time_t, jobject>& compiledscript_pair = JAVA->compiledscript_instances[md5key];
                            if (difftime(now, compiledscript_pair.first) > conf->javascript_compiledscript_expires) {
                                JAVA->env->DeleteLocalRef(compiledscript_pair.second);
                                JAVA->compiledscript_instances.erase(md5key);
                                goto update_file_compiledscript_instance;
                            }
                            if (compiledscript_pair.second != NULL) {
                                JAVA->env->CallObjectMethod(compiledscript_pair.second, JAVA->compiledscript_eval_void);
                            }
                        } else {
update_file_compiledscript_instance:
                            jstring javascript_path = JAVA->env->NewStringUTF(script_path.c_str());
                            jobject filereader_instance = (jobject) JAVA->env->NewObject(JAVA->filereader, JAVA->filereader_ctor, javascript_path);

                            if (engine.second != NULL) {
                                jobject compiledscript_instance = (jobject) JAVA->env->CallObjectMethod(engine.second, JAVA->compile_filereader, filereader_instance);
                                if (compiledscript_instance != NULL) {
                                    JAVA->compiledscript_instances[md5key] = std::make_pair(time(0), compiledscript_instance);
                                    JAVA->env->CallObjectMethod(compiledscript_instance, JAVA->compiledscript_eval_void);
                                }
                            } else {
                                JAVA->env->CallObjectMethod(engine.first, JAVA->script_engine_eval_filereader, filereader_instance);
                            }
                            JAVA->env->DeleteLocalRef(filereader_instance);
                            JAVA->env->ReleaseStringUTFChars(javascript_path, 0);
                            JAVA->env->DeleteLocalRef(javascript_path);
                        }
                    }
                }

            } else if (conf->javascript_content.len > 0) {

                jstring script_content = JAVA->env->NewStringUTF((char*) conf->javascript_content.data);

                if (engine.second != NULL) {
                    jobject compiledscript_instance = (jobject) JAVA->env->CallObjectMethod(engine.second, JAVA->compile_string, script_content);
                    if (compiledscript_instance != NULL) {
                        JAVA->env->CallObjectMethod(compiledscript_instance, JAVA->compiledscript_eval_void);
                        JAVA->env->DeleteLocalRef(compiledscript_instance);
                    }
                } else {

                    JAVA->env->CallObjectMethod(engine.first, JAVA->script_engine_eval_string, script_content);
                }

                JAVA->env->ReleaseStringUTFChars(script_content, 0);
                JAVA->env->DeleteLocalRef(script_content);
            }



            java_output_handler(conf, req, res, request_instance, response_instance);

            JAVA->env->DeleteLocalRef(request_instance);
            JAVA->env->DeleteLocalRef(response_instance);

            JAVA->env->ReleaseStringUTFChars(jhi_req, 0);
            JAVA->env->DeleteLocalRef(jhi_req);
            JAVA->env->ReleaseStringUTFChars(jhi_res, 0);
            JAVA->env->DeleteLocalRef(jhi_res);
        }
    }
}


#endif /* JAVA_HANDLER_HPP */

