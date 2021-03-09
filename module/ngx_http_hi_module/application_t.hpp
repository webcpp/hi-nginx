#pragma once

enum application_t
{
    __cpp__,
#ifdef HTTP_HI_PYTHON
    __python__,
#endif
#ifdef HTTP_HI_LUA
    __lua__,
#endif
#ifdef HTTP_HI_JAVA
    __java__,
#endif
#ifdef HTTP_HI_QJS
    __qjs__,
#endif
    __unkown__
};
