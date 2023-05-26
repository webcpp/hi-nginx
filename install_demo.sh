#!/bin/bash
if ! test -f Makefile;then
./configure                                                         \
                --with-cc=clang                                     \
                --with-http_ssl_module                              \
                --with-http_v2_module                               \
                --with-http_v3_module                               \
                --with-http_gzip_static_module                      \
                --with-stream                                       \
                --with-stream_ssl_module                            \
                --with-http_realip_module                           \
                --prefix=/usr/local/nginx                           \
                --with-http-hi-python-version=python-3.8-embed      \
                --with-http-hi-lua-version=lua5.3                   \
                --add-module=module/ngx_http_autoblacklist_module   \
                --add-module=module/ngx_http_lua_module             \
                --add-module=module/ngx_http_py_module              \
                --add-module=module/ngx_http_java_module            \
                --add-module=module/ngx_http_cpp_module    

else
    echo ready.
fi
