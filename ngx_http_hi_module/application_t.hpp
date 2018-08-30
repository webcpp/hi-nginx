#ifndef APPLICATION_T_HPP
#define APPLICATION_T_HPP

enum application_t {
    __cpp__,
#ifdef HTTP_HI_PYTHON
    __python__,
#endif
#ifdef HTTP_HI_LUA
    __lua__,
#endif
#ifdef HTTP_HI_JAVA    
    __java__,
    __javascript__,
#endif
#ifdef HTTP_HI_PHP
    __php__,
#endif
    __unkown__
};


#endif /* APPLICATION_T_HPP */

