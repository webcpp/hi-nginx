#include "java.hpp"

namespace hi
{

    java::java(const std::string &classpath, const std::string &option, int version, const std::string &servlet)
        : jvm(0), args(), options(0), ok(false), inited(false), servlet(), env(0), version(11), request(0), response(0), hashmap(0), arraylist(0), iterator(0), set(0), request_ctor(0), response_ctor(0), hashmap_put(0), hashmap_get(0), hashmap_keyset(0), arraylist_get(0), arraylist_size(0), arraylist_iterator(0), hasnext(0), next(0), set_iterator(0), status(0), content(0), client(0), user_agent(0), method(0), uri(0), param(0), req_headers(0), form(0), cookies(0), req_session(0), req_cache(0), res_headers(0), res_session(0), res_cache(0)
    {
        this->init(classpath, option, version, servlet);
    }
    java::~java()
    {
        if (this->ok)
        {
            this->free_vm();
        }
        if (this->options)
        {
            free(this->options);
        }

        this->request = 0;
        this->response = 0;
        this->hashmap = 0;
        this->arraylist = 0;
        this->iterator = 0;
        this->set = 0;

        this->request_ctor = 0;
        this->response_ctor = 0;
        this->hashmap_get = 0;
        this->hashmap_put = 0;
        this->hashmap_keyset = 0;
        this->arraylist_get = 0;
        this->arraylist_size = 0;
        this->arraylist_iterator = 0;
        this->hasnext = 0;
        this->next = 0;
        this->set_iterator = 0;
        this->status = 0;
        this->content = 0;
        this->client = 0;
        this->user_agent = 0;
        this->method = 0;
        this->uri = 0;
        this->param = 0;
        this->req_headers = 0;
        this->form = 0;
        this->cookies = 0;
        this->req_session = 0;
        this->req_cache = 0;
        this->res_headers = 0;
        this->res_session = 0;
        this->res_cache = 0;
    }

    void java::init(const std::string &classpath, const std::string &option, int version, const std::string &servlet)
    {
        this->version = version;
        this->ok = this->create_vm(classpath, option);

        this->inited = this->init_jvm(servlet);
    }
    bool java::is_ok() const
    {
        return this->ok && this->inited;
    }

    bool java::create_vm(const std::string &classpath, const std::string &jvmoptions)
    {
        JNI_GetDefaultJavaVMInitArgs(&this->args);
        switch (this->version)
        {
        case 1:
            this->args.version = JNI_VERSION_1_1;
            break;
        case 2:
            this->args.version = JNI_VERSION_1_2;
            break;
        case 4:
            this->args.version = JNI_VERSION_1_4;
            break;
        case 6:
            this->args.version = JNI_VERSION_1_6;
            break;
        case 8:
            this->args.version = JNI_VERSION_1_8;
            break;
#ifdef JNI_VERSION_9
        case 9:
            this->args.version = JNI_VERSION_9;
            break;
#endif
#ifdef JNI_VERSION_10
        case 10:
            this->args.version = JNI_VERSION_10;
            break;
#endif
#ifdef JNI_VERSION_19
        case 19:
            this->args.version = JNI_VERSION_19;
            break;
#endif
        default:
#ifdef JNI_VERSION_20
            this->args.version = JNI_VERSION_20;
#else
            this->args.version = JNI_VERSION_1_8;
#endif
            break;
        }
        std::vector<std::string> opt_vec;
        // this->split(classpath, opt_vec, " ");
        // this->split(jvmoptions, opt_vec, " ");
        split(classpath, std::ref(opt_vec), " ");
        split(jvmoptions, std::ref(opt_vec), " ");
        this->args.nOptions = opt_vec.size();
        this->options = (JavaVMOption *)calloc(sizeof(JavaVMOption), this->args.nOptions);
        if (this->options == NULL)
        {
            return false;
        }
        for (size_t i = 0; i < this->args.nOptions; ++i)
        {
            this->options[i].optionString = const_cast<char *>(opt_vec[i].data());
        }
        this->args.options = this->options;
        this->args.ignoreUnrecognized = JNI_TRUE;
        int rv = JNI_CreateJavaVM(&this->jvm, (void **)&this->env, &this->args);
        return (rv < 0 || !this->env) ? false : true;
    }
    void java::split(const std::string &str, std::vector<std::string> &v, const char *delimiters)
    {
        char tmp[str.size()];
        strcpy(tmp, str.data());
        char *p = strtok(tmp, delimiters);
        while (p)
        {
            v.push_back(p);
            p = strtok(NULL, delimiters);
        }
    }
    void java::free_vm()
    {
        this->jvm->DestroyJavaVM();
    }

    bool java::init_jvm(const std::string &servlet)
    {
        this->request = this->env->FindClass("hi/request");
        if (this->request != NULL)
        {
            this->request_ctor = this->env->GetMethodID(this->request, "<init>", "()V");
            this->client = this->env->GetFieldID(this->request, "client", "Ljava/lang/String;");
            this->user_agent = this->env->GetFieldID(this->request, "user_agent", "Ljava/lang/String;");
            this->method = this->env->GetFieldID(this->request, "method", "Ljava/lang/String;");
            this->uri = this->env->GetFieldID(this->request, "uri", "Ljava/lang/String;");
            this->param = this->env->GetFieldID(this->request, "param", "Ljava/lang/String;");
            this->req_headers = this->env->GetFieldID(this->request, "headers", "Ljava/util/HashMap;");
            this->form = this->env->GetFieldID(this->request, "form", "Ljava/util/HashMap;");
            this->cookies = this->env->GetFieldID(this->request, "cookies", "Ljava/util/HashMap;");
            this->req_session = this->env->GetFieldID(this->request, "session", "Ljava/util/HashMap;");
            this->req_cache = this->env->GetFieldID(this->request, "cache", "Ljava/util/HashMap;");

            this->response = this->env->FindClass("hi/response");
            if (this->response != NULL)
            {
                this->response_ctor = this->env->GetMethodID(this->response, "<init>", "()V");
                this->status = this->env->GetFieldID(this->response, "status", "I");
                this->content = this->env->GetFieldID(this->response, "content", "Ljava/lang/String;");
                this->res_headers = this->env->GetFieldID(this->response, "headers", "Ljava/util/HashMap;");
                this->res_session = this->env->GetFieldID(this->response, "session", "Ljava/util/HashMap;");
                this->res_cache = this->env->GetFieldID(this->response, "cache", "Ljava/util/HashMap;");

                this->hashmap = this->env->FindClass("java/util/HashMap");
                this->hashmap_put = this->env->GetMethodID(this->hashmap, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
                this->hashmap_get = this->env->GetMethodID(this->hashmap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
                this->hashmap_keyset = this->env->GetMethodID(this->hashmap, "keySet", "()Ljava/util/Set;");

                this->arraylist = this->env->FindClass("java/util/ArrayList");
                this->arraylist_get = this->env->GetMethodID(this->arraylist, "get", "(I)Ljava/lang/Object;");
                this->arraylist_size = this->env->GetMethodID(this->arraylist, "size", "()I");
                this->arraylist_iterator = this->env->GetMethodID(this->arraylist, "iterator", "()Ljava/util/Iterator;");

                this->iterator = this->env->FindClass("java/util/Iterator");
                this->hasnext = this->env->GetMethodID(this->iterator, "hasNext", "()Z");
                this->next = this->env->GetMethodID(this->iterator, "next", "()Ljava/lang/Object;");

                this->set = this->env->FindClass("java/util/Set");
                this->set_iterator = this->env->GetMethodID(this->set, "iterator", "()Ljava/util/Iterator;");

                this->servlet.SERVLET = this->env->FindClass(servlet.c_str());
                if (this->servlet.SERVLET != NULL)
                {
                    this->servlet.CTOR = this->env->GetMethodID(this->servlet.SERVLET, "<init>", "()V");
                    this->servlet.GET_INSTANCE = this->env->GetStaticMethodID(this->servlet.SERVLET, "get_instance", "()Lhi/servlet;");
                    this->servlet.HANDLER = this->env->GetMethodID(this->servlet.SERVLET, "handler", "(Lhi/request;Lhi/response;)V");
                    this->servlet.t = time(0);
                    return true;
                }
            }
        }

        return false;
    }

    void java::input(const hi::request &req, hi::response &res, jobject request_instance, jobject response_instance)
    {
        jstring client = this->env->NewStringUTF(req.client.c_str());
        this->env->SetObjectField(request_instance, this->client, client);
        this->env->ReleaseStringUTFChars(client, 0);
        this->env->DeleteLocalRef(client);

        jstring user_agent = this->env->NewStringUTF(req.user_agent.c_str());
        this->env->SetObjectField(request_instance, this->user_agent, user_agent);
        this->env->ReleaseStringUTFChars(user_agent, 0);
        this->env->DeleteLocalRef(user_agent);

        jstring method = this->env->NewStringUTF(req.method.c_str());
        this->env->SetObjectField(request_instance, this->method, method);
        this->env->ReleaseStringUTFChars(method, 0);
        this->env->DeleteLocalRef(method);

        jstring uri = this->env->NewStringUTF(req.uri.c_str());
        this->env->SetObjectField(request_instance, this->uri, uri);
        this->env->ReleaseStringUTFChars(uri, 0);
        this->env->DeleteLocalRef(uri);

        jstring param = this->env->NewStringUTF(req.param.c_str());
        this->env->SetObjectField(request_instance, this->param, param);
        this->env->ReleaseStringUTFChars(param, 0);
        this->env->DeleteLocalRef(param);

        jobject req_headers = this->env->GetObjectField(request_instance, this->req_headers);
        for (auto &item : req.headers)
        {
            jstring k = this->env->NewStringUTF(item.first.c_str()), v = this->env->NewStringUTF(item.second.c_str());
            this->env->CallObjectMethod(req_headers, this->hashmap_put, k, v);
            this->env->ReleaseStringUTFChars(k, 0);
            this->env->ReleaseStringUTFChars(v, 0);
            this->env->DeleteLocalRef(k);
            this->env->DeleteLocalRef(v);
        }
        this->env->DeleteLocalRef(req_headers);

        jobject req_form = this->env->GetObjectField(request_instance, this->form);
        for (auto &item : req.form)
        {
            jstring k = this->env->NewStringUTF(item.first.c_str()), v = this->env->NewStringUTF(item.second.c_str());
            this->env->CallObjectMethod(req_form, this->hashmap_put, k, v);
            this->env->ReleaseStringUTFChars(k, 0);
            this->env->ReleaseStringUTFChars(v, 0);
            this->env->DeleteLocalRef(k);
            this->env->DeleteLocalRef(v);
        }
        this->env->DeleteLocalRef(req_form);

        jobject req_cookies = this->env->GetObjectField(request_instance, this->cookies);
        for (auto &item : req.cookies)
        {
            jstring k = this->env->NewStringUTF(item.first.c_str()), v = this->env->NewStringUTF(item.second.c_str());
            this->env->CallObjectMethod(req_cookies, this->hashmap_put, k, v);
            this->env->ReleaseStringUTFChars(k, 0);
            this->env->ReleaseStringUTFChars(v, 0);
            this->env->DeleteLocalRef(k);
            this->env->DeleteLocalRef(v);
        }
        this->env->DeleteLocalRef(req_cookies);
    }

    void java::output(const hi::request &req, hi::response &res, jobject request_instance, jobject response_instance)
    {
        jobject res_headers = this->env->GetObjectField(response_instance, this->res_headers);
        jobject keyset = this->env->CallObjectMethod(res_headers, this->hashmap_keyset);
        jobject iterator = this->env->CallObjectMethod(keyset, this->set_iterator);
        while ((bool)this->env->CallBooleanMethod(iterator, this->hasnext))
        {
            jstring k = (jstring)this->env->CallObjectMethod(iterator, this->next);
            jobject item = this->env->CallObjectMethod(res_headers, this->hashmap_get, k);
            jobject jterator = this->env->CallObjectMethod(item, this->arraylist_iterator);
            const char *kstr = this->env->GetStringUTFChars(k, NULL);
            while ((bool)this->env->CallBooleanMethod(jterator, this->hasnext))
            {
                jstring v = (jstring)this->env->CallObjectMethod(jterator, this->next);
                const char *vstr = this->env->GetStringUTFChars(v, NULL);
                if (strcmp(kstr, "Content-Type") == 0)
                {
                    res.headers.find(kstr)->second = vstr;
                }
                else
                {
                    res.headers.insert(std::make_pair(kstr, vstr));
                }

                this->env->ReleaseStringUTFChars(v, vstr);
                this->env->DeleteLocalRef(v);
            }
            this->env->ReleaseStringUTFChars(k, kstr);
            this->env->DeleteLocalRef(k);
            this->env->DeleteLocalRef(item);
            this->env->DeleteLocalRef(jterator);
        }
        this->env->DeleteLocalRef(res_headers);
        this->env->DeleteLocalRef(keyset);
        this->env->DeleteLocalRef(iterator);

        res.status = this->env->GetIntField(response_instance, this->status);
        jstring content = (jstring)this->env->GetObjectField(response_instance, this->content);
        const char *contentstr = this->env->GetStringUTFChars(content, NULL);
        res.content = contentstr;
        this->env->ReleaseStringUTFChars(content, contentstr);
        this->env->DeleteLocalRef(content);
    }
    void java::main(const hi::request &req, hi::response &res)
    {
        if (this->ok && this->inited)
        {
            jobject request_instance, response_instance;

            request_instance = this->env->NewObject(this->request, this->request_ctor);
            response_instance = this->env->NewObject(this->response, this->response_ctor);

            this->input(req, res, request_instance, response_instance);
            jobject servlet_instance;
            if (this->servlet.GET_INSTANCE == NULL)
            {
                servlet_instance = this->env->NewObject(this->servlet.SERVLET, this->servlet.CTOR);
            }
            else
            {
                servlet_instance = this->env->CallStaticObjectMethod(this->servlet.SERVLET, this->servlet.GET_INSTANCE);
            }
            this->env->CallVoidMethod(servlet_instance, this->servlet.HANDLER, request_instance, response_instance);
            this->env->DeleteLocalRef(servlet_instance);

            this->output(req, res, request_instance, response_instance);

            this->env->DeleteLocalRef(request_instance);
            this->env->DeleteLocalRef(response_instance);
        }
    }

} // namespace hi
