#!/bin/bash
./configure     --with-http_ssl_module \
                --with-http_v2_module \
                --prefix=/home/centos7/nginx \
                --add-module=ngx_http_hi_module  
