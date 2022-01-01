#pragma once

#include <ctime>
#include <jni.h>

namespace hi
{
    class java_servlet
    {
    public:
        jclass SERVLET;
        jmethodID CTOR, HANDLER, GET_INSTANCE;
        time_t t;

        java_servlet();

        java_servlet(const java_servlet &other);

        java_servlet &operator=(const java_servlet &right);

        virtual ~java_servlet();
    };
} // namespace hi