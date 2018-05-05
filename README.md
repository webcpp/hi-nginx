# Introduction
A distribution of Nginx with c++,python,lua java and php web development. 


[hi-nginx-demo](https://github.com/webcpp/hi_demo).

[hi-nginx-gitbook](https://doc.hi-nginx.com/).

[hi-nginx-python-docs](http://www.cnblogs.com/hi-nginx/)

[hi.py web framework for hi-nginx ](https://github.com/webcpp/hi.py)

# Features
- All features of nginx-1.14.0(latest release) are inherited, i.e., it is 100% compatible with nginx.
- Web development using python, c++, lua, php7 and java

# Dependency
- linux
- gcc and g++(c++11) or clang and clang++
- hiredis-devel
- python-devel,if `--enable-http-hi-python=YES` and `with-http-hi-python-version=python2`
- python3.x-devel if `--enable-http-hi-python=YES` and `with-http-hi-python-version=python3`
- lua-devel(lua5.x),if `--enable-http-hi-lua=YES`  and `--with-http-hi-lua-version=lua5.x`
- luajit-devel,if `--enable-http-hi-lua=YES` and `--with-http-hi-lua-version=luajit`
- jdk 1.8,9,10,if `--enable-http-hi-java=YES`
- PHP 7.0 or later(--enable-embed=shared),if `--enable-http-hi-php=YES`

## centos
```
sudo yum install gcc gcc-c++ make pcre-devel zlib-devel openssl-devel hiredis-devel python-devel python3.x-devel lua-devel

```

## ubuntu
```
sudo apt-get install build-essential libpcre3-dev zlib1g-dev libssl-dev libhiredis-dev python-dev python3-dev liblua5.3-dev 

```

## About jdk

Create the file /etc/ld.so.conf.d/java.conf with the following entries，replace $JAVA_HOME as real path:
```
$JAVA_HOME/lib/
$JAVA_HOME/lib/server
```

If you install jdk 1.8, then replace the above code with the following code:
```
$JAVA_HOME/jre/lib/amd64/server
```

Then
```
sudo ldconfig
```

## About php7+ install

Please change `configure` file :

```
ac_fn_c_check_decl "$LINENO" "isfinite" "ac_cv_have_decl_isfinite" "#include <math.h>
"
if test "x$ac_cv_have_decl_isfinite" = xyes; then :
  ac_have_decl=0
else
  ac_have_decl=0
fi

```

then 

`configure --enable-embed=shared`.

example:

```

./configure --enable-embed=shared --prefix=/usr/local --enable-mbstring  --enable-mysqlnd --with-mysqli=mysqlnd --with-pdo-mysql=mysqlnd --enable-opcache --with-openssl --with-zlib --enable-simplexml --enable-xmlreader --enable-xmlwriter --enable-bcmath -with-curl --with-gd

```

add `/usr/local/lib` to `/etc/ld.so.conf.d/php.conf` and `ldconfig`


change php.ini:

```

include_path = ".:/usr/local/nginx/php"

```




# Installation
`./configure --help` or see `install_demo.sh` or 

```shell
                --enable-http-hi-cpp=YES                            \
                --enable-http-hi-python=YES                         \
                --enable-http-hi-lua=YES                            \
                --enable-http-hi-java=YES                           \
                --enable-http-hi-php=YES                            \
                --with-http-hi-python-version=python3               \
                --with-http-hi-lua-version=lua5.3                   \
                --add-module=ngx_http_hi_module                     \

```


# python and lua api
## hi_req
- uri
- method
- client
- param
- user_agent
- has_header
- get_header
- has_form
- get_form
- has_session
- get_session
- has_cookie
- get_cookie
## hi_res
- status
- content
- header
- session

# hello,world

## cpp servlet class

```cpp
#include "servlet.hpp"
namespace hi{
class hello : public servlet {
    public:

        void handler(request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
            res.content = "hello,world";
            res.status = 200;
        }

    };
}

extern "C" hi::servlet* create() {
    return new hi::hello();
}

extern "C" void destroy(hi::servlet* p) {
    delete p;
}

```

### cpp compile

```
g++ -std=c++11 -I/usr/local/nginx/include  -shared -fPIC hello.cpp -o hello.so
install hello.so /usr/local/nginx/cpp

```

## java servlet class

```java
package hi;

public class jhello implements hi.servlet {

    public jhello() {

    }

    public void handler(hi.request req, hi.response res) {
        res.status = 200;
        res.content = "hello,world";

    }
}

```

### java compile

```
${JAVA_HOME}/bin/javac -classpath .:${NGINX_INSTALL_DIR}/java/hi-nginx-java.jar hi/jhello.java

```


## php servlet class

see `php/hi/request.php`,`php/hi/response.php` and `php/hi/servlet.php`

```php

<?php

require_once 'hi/servlet.php';
require_once 'hi/route.php';

class index implements \hi\servlet {

    public function handler(\hi\request $req, \hi\response $res) {
        $app = \hi\route::get_instance();
        $app->add('{^/$}', array('GET'), function ($rq, $rs, &$param) {
            $rs->content = 'hello,world';
            $rs->status = 200;
        });
        
        $app->add('{^/who/(?P<name>\w+)/?$}', array('GET'), function ($rq, $rs, &$param) {
            $rs->content = 'hello,'.$param['name'];
            $rs->status = 200;
        });
        $app->run($req, $res);
    }

}

```




# 3rd party module

  -  nginx-rtmp-module
  -  nchan

# ngx_http_hi_module directives
- directives : content: loc,if in loc
    - hi,default: ""

    example:
    
```
            location = /hello {
                hi cpp/hello.so ;
            }
```
- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_need_cache,default: on

    example:

```
        hi_need_cache on|off;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_cache_size,default: 10

    example:

```
        hi_cache_size 10;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_cache_expires,default: 300s

    example:

```
        hi_cache_expires 300s;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_need_headers,default: off

        example:

```
        hi_need_headers on|off;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_need_cookies,default: off

        example:

```
        hi_need_cookies on|off;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_need_session,default: off

        example:

```
        hi_need_session on|off;
```

- directives : content: http,srv,loc,if in loc ,if in srv
    - hi_session_expires,default: 300s

    example:

```
        hi_session_expires 300s;
```
     

- directives : content: http,srv ,if in srv
    - hi_redis_host,default: ""

    example:

```
        hi_redis_host 127.0.0.1;
```

- directives : content: http,srv,if in srv
    - hi_redis_port,default: 0

    example:

```
        hi_redis_port 6379;
```


- directives : content: loc,if in loc
    - hi_python_content,default: ""

    example:
    
```
            location = /pyecho {
                hi_python_content "hi_res.status(200)\nhi_res.content('hello,world')" ;
            }
```

- directives : content: loc,if in loc
    - hi_python_script,default: ""

    example:
    
```
            location ~ \.py$  {
                hi_python_script python;
            }
```

or

```
            location / {
                hi_python_script python/index.py;
            }
```


- directives : content:  http,srv,if in srv
    - hi_lua_package_path,default: ""

    example:
    
```

            hi_lua_package_path '/usr/local/nginx/lua/?.lua;';

```

- directives : content:  http,srv,if in srv
    - hi_lua_package_cpath,default: ""

    example:
    
```

            hi_lua_package_cpath '/usr/local/nginx/lua/?.so;';

```




- directives : content: loc,if in loc
    - hi_lua_content,default: ""

    example:
    
```
            location = /luaecho {
                hi_lua_content "hi_res:status(200)\nhi_res:content('hello,world')" ;
            }
```

- directives : content: loc,if in loc
    - hi_lua_script,default: ""

    example:
    
```
            location ~ \.lua$  {
                hi_lua_script lua;
            }
```

or

```
            location / {
                hi_lua_script lua/index.lua;
            }
```

- directives : content: http,srv,if in srv
    - hi_java_classpath,default:"-Djava.class.path=."

    example:

```
hi_java_classpath "-Djava.class.path=.:/usr/local/nginx/java:/usr/local/nginx/java/hi-nginx-java.jar";

```

- directives : content: http,srv,if in srv
    - hi_java_options,default:"-server -d64 -Xmx1G -Xms1G -Xmn256m"

    example:

```
hi_java_options "-server -d64 -Xmx3G -Xms3G -Xmn768m -XX:+DisableExplicitGC -XX:+UseConcMarkSweepGC -XX:+UseParNewGC -XX:+UseNUMA -XX:+CMSParallelRemarkEnabled -XX:MaxTenuringThreshold=15 -XX:MaxGCPauseMillis=30 -XX:GCPauseIntervalMillis=150 -XX:+UseAdaptiveGCBoundary -XX:-UseGCOverheadLimit -XX:+UseBiasedLocking -XX:SurvivorRatio=8 -XX:TargetSurvivorRatio=90 -XX:MaxTenuringThreshold=15 -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -XX:+UseFastAccessorMethods -XX:+UseCompressedOops -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:ReservedCodeCacheSize=2048m -XX:+UseCodeCacheFlushing -XX:SoftRefLRUPolicyMSPerMB=10000 -XX:ParallelGCThreads=10";

```

- directives : content: loc,if in loc
    - hi_java_servlet,default:""

    example:

```
hi_java_servlet hi/jhello;


```

- directives : content : http,srv,loc,if in loc ,if in srv
    - hi_java_servlet_cache_expires,default:300s

    example:

```
hi_java_servlet_cache_expires 300s;

```

- directives : content : http,srv,loc,if in loc ,if in srv
    - hi_java_servlet_cache_size,default:10

    example:

```
hi_java_servlet_cache_size 10;

```

- directives : content: http,srv,if in srv
    - hi_java_version,default:8

    example:

```
hi_java_version 8;

```

- directives : content: loc,if in loc
    - hi_php_script,default: ""

    example:
    
```
            location ~ \.php$  {
                hi_php_script php;
            }
```

or

```
            location / {
                hi_php_script php/index.php;
            }
```
## nginx.conf

[hi_demo_conf](https://github.com/webcpp/hi_demo/blob/master/demo.conf)