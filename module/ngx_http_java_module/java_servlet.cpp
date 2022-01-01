#include "java_servlet.hpp"

namespace hi
{
    java_servlet::java_servlet()
        : SERVLET(0), CTOR(0), HANDLER(0), GET_INSTANCE(0), t(time(0))
    {
    }

    java_servlet::java_servlet(const java_servlet &other)
        : SERVLET(other.SERVLET), CTOR(other.CTOR), HANDLER(other.HANDLER), GET_INSTANCE(other.GET_INSTANCE), t(other.t)
    {
    }

    java_servlet &java_servlet::java_servlet::operator=(const java_servlet &right)
    {
        if (this == &right)
        {
            return *this;
        }
        this->SERVLET = right.SERVLET;
        this->CTOR = right.CTOR;
        this->HANDLER = right.HANDLER;
        this->GET_INSTANCE = right.GET_INSTANCE;
        this->t = right.t;
        return *this;
    }
    java_servlet::~java_servlet()
    {
        this->SERVLET = 0;
        this->CTOR = 0;
        this->HANDLER = 0;
        this->GET_INSTANCE = 0;
    }
} //namespace hi