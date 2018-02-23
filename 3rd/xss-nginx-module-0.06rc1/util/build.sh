#!/bin/bash

# this file is mostly meant to be used by the author himself.

ragel -T1 src/ngx_http_xss_util.rl || exit 1
if [ $? != 0 ]; then
    echo 'Failed to generate the ngx_http_xss_util.c.' 1>&2
    exit 1;
fi

./util/fix-clang-warnings || exit 1

root=`pwd`
version=$1
force=$2

ngx-build $force $version \
            --with-cc-opt="-I$PCRE_INC -I$OPENSSL_INC" \
            --with-ld-opt="-L$PCRE_LIB -L$OPENSSL_LIB -Wl,-rpath,$PCRE_LIB:$OPENSSL_LIB" \
        --with-http_ssl_module \
            --without-mail_pop3_module \
            --without-mail_imap_module \
            --without-mail_smtp_module \
            --without-http_upstream_ip_hash_module \
            --without-http_empty_gif_module \
            --without-http_memcached_module \
            --without-http_referer_module \
            --without-http_autoindex_module \
            --without-http_auth_basic_module \
            --without-http_userid_module \
          --add-module=../echo-nginx-module \
          --add-module=../ndk-nginx-module \
          --add-module=../lua-nginx-module \
          --add-module=$root $opts \
          --with-debug
          #--with-cc-opt="-g3 -O0"
          #--add-module=$root/../echo-nginx-module \
  #--without-http_ssi_module  # we cannot disable ssi because echo_location_async depends on it (i dunno why?!)

