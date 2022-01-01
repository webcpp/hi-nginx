#pragma once

#include <jni.h>
#include <string>
#include <cstring>
#include <ctime>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include "utils.hpp"
#include "request.hpp"
#include "response.hpp"
#include "servlet.hpp"
#include "java_servlet.hpp"

namespace hi
{

    class java
    {
    public:
        java();
        java(const std::string& classpath,const std::string& option,int version,const std::string& servlet);
        virtual ~java();
        void init(const std::string& classpath,const std::string& option,int version,const std::string& servlet);
        bool is_ok() const;

        void main(const request &req, response &res);

    private:
        bool init_jvm(const std::string& servlet);
        void input(const request &req, response &res, jobject request_instance, jobject response_instance);

        void output(const request &req, response &res, jobject request_instance, jobject response_instance);

    private:
        JavaVM *jvm;
        JavaVMInitArgs args;
        JavaVMOption *options;
        bool ok, inited;
        java_servlet servlet;

    private:
        JNIEnv *env;
        int version;
        jclass request, response, hashmap, arraylist, iterator, set;
        jmethodID request_ctor, response_ctor, hashmap_put, hashmap_get, hashmap_keyset, arraylist_get, arraylist_size, arraylist_iterator, hasnext, next, set_iterator;
        jfieldID status, content, client, user_agent, method, uri, param, req_headers, form, cookies, req_session, req_cache, res_headers, res_session, res_cache;

    private:
        bool create_vm(const std::string &classpath, const std::string &jvmoptions);
        void split(const std::string &str, std::vector<std::string> &v, const char *delimiters);

        void free_vm();
    };

} // namespace hi
