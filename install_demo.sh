#!/bin/bash
if ! test -f Makefile;then
./configure                                                         \
                --with-cc=gcc                                       \
                --with-http_ssl_module                              \
                --with-http_v2_module                               \
                --with-http_gzip_static_module                      \
                --with-http_stub_status_module                      \
                --with-stream                                       \
                --with-stream_ssl_module                            \
                --with-http_realip_module                           \
                --prefix=/usr/local/nginx                           \
                --enable-http-hi-cpp=YES                            \
                --enable-http-hi-python=YES                         \
                --enable-http-hi-qjs=YES                            \
                --enable-http-hi-lua=NO                            \
                --enable-http-hi-java=YES                           \
                --with-http-hi-python-version=python-3.8-embed      \
                --with-http-hi-lua-version=lua5.3                   \
                --add-module=module/ngx_http_hi_module              \
                --add-module=module/ngx_http_autoblacklist_module   \
                --add-module=3rd/ngx_http_concat_module             \
                --add-module=3rd/ngx_http_footer_filter_module      \
                --add-module=3rd/ngx_http_trim_filter_module        \
                --add-module=3rd/nginx-push-stream-module-0.5.4     \
                --add-module=3rd/ngx_slab_stat

else
    echo ready.
fi
