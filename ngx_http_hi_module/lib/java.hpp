#ifndef JAVA_HPP
#define JAVA_HPP

#include <jni.h>
#include <string>
#include <ctime>

namespace hi {

    class java_servlet_t {
    public:
        jclass SERVLET;
        jmethodID CTOR, HANDLER;
        time_t t;

        java_servlet_t() :
        SERVLET(0), CTOR(0), HANDLER(0), t(time(0)) {
        }

        java_servlet_t(const java_servlet_t& other) :
        SERVLET(other.SERVLET)
        , CTOR(other.CTOR)
        , HANDLER(other.HANDLER)
        , t(other.t) {
        }

        java_servlet_t& operator=(const java_servlet_t& right) {
            if (this == &right) {
                return *this;
            }
            this->SERVLET = right.SERVLET;
            this->CTOR = right.CTOR;
            this->HANDLER = right.HANDLER;
            this->t = right.t;
            return *this;
        }

        virtual ~java_servlet_t() {
            this->SERVLET = 0;
            this->CTOR = 0;
            this->HANDLER = 0;
        }



    };

    class java {
    public:

        java(const std::string& classpath, const std::string& jvmoptions, int v)
        : jvm(0)
        , args()
        , options()
        , ok(false)
        , env(0)
        , version(v)
        , request(0), response(0), hashmap(0), arraylist(0), iterator(0), set(0)
        , request_ctor(0), response_ctor(0), hashmap_put(0), hashmap_get(0), hashmap_keyset(0), arraylist_get(0), arraylist_size(0), arraylist_iterator(0), hasnext(0), next(0), set_iterator(0)
        , status(0), content(0)
        , client(0), user_agent(0), method(0), uri(0), param(0)
        , req_headers(0), form(0), cookies(0), req_session(0)
        , res_headers(0), res_session(0) {
            this->ok = this->create_vm(classpath, jvmoptions);
        }

        virtual~java() {
            if (this->ok) {
                this->free_vm();
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
            this->res_headers = 0;
            this->res_session = 0;

        }

        bool is_ok()const {
            return this->ok;
        }




    private:
        JavaVM *jvm;
        JavaVMInitArgs args;
        JavaVMOption options[2];
        bool ok;
    public:
        JNIEnv *env;
        int version;
        jclass request, response, hashmap, arraylist, iterator, set;
        jmethodID request_ctor, response_ctor, hashmap_put, hashmap_get, hashmap_keyset, arraylist_get, arraylist_size, arraylist_iterator, hasnext, next, set_iterator;
        jfieldID status, content, client, user_agent, method, uri, param, req_headers, form, cookies, req_session, res_headers, res_session;
    private:

        bool create_vm(const std::string& classpath, const std::string& jvmoptions) {
            switch (this->version) {
                case 1:this->args.version = JNI_VERSION_1_1;
                    break;
                case 2:this->args.version = JNI_VERSION_1_2;
                    break;
                case 4:this->args.version = JNI_VERSION_1_4;
                    break;
                case 6:this->args.version = JNI_VERSION_1_6;
                    break;
                case 8:this->args.version = JNI_VERSION_1_8;
                    break;
                case 9:this->args.version = JNI_VERSION_9;
                    break;
                default:this->args.version = JNI_VERSION_9;
                    break;
            }
            this->args.nOptions = 2;
            this->options[0].optionString = const_cast<char*> (classpath.c_str());
            this->options[1].optionString = const_cast<char*> (jvmoptions.c_str());

            this->args.options = this->options;
            this->args.ignoreUnrecognized = JNI_TRUE;
            int rv = JNI_CreateJavaVM(&this->jvm, (void**) & this->env, &this->args);
            return (rv < 0 || !this->env) ? false : true;
        }

        void free_vm() {
            this->jvm->DestroyJavaVM();
        }


    };
}

#endif /* JAVA_HPP */

