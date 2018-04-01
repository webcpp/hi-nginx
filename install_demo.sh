#!/bin/bash
if ! test -f Makefile;then
./configure                                                         \
                --with-cc=gcc                                       \
                --with-http_ssl_module                              \
                --with-http_v2_module                               \
                --prefix=/usr/local/nginx                           \
                --enable-http-hi-cpp=YES                            \
                --enable-http-hi-python=YES                         \
                --enable-http-hi-lua=YES                            \
                --enable-http-hi-java=YES                           \
                --enable-http-hi-php=YES                            \
                --with-http-hi-python-version=3                     \
                --with-http-hi-lua-version=lua                      \
                --add-module=ngx_http_hi_module                     \
                --add-module=3rd/nginx-http-concat-1.2.2            \
                --add-module=3rd/nchan-1.1.14                       \
                --add-module=3rd/nginx-rtmp-module-1.2.1            
else
    echo ready.
fi
