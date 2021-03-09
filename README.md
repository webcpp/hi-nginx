# Features
- All features of nginx(latest release) are inherited, i.e., it is 100% compatible with nginx.
- Web development using python, c++, lua, java , quickjs

# Dependency
- linux
- gcc and g++(c++11) or clang and clang++
- python-devel,if `--enable-http-hi-python=YES` and `with-http-hi-python-version=python2`
- python3.x-devel if `--enable-http-hi-python=YES` and `with-http-hi-python-version=python3(or python3-embed)`
- lua-devel(lua5.x),if `--enable-http-hi-lua=YES`  and `--with-http-hi-lua-version=lua5.x`
- luajit-devel,if `--enable-http-hi-lua=YES` and `--with-http-hi-lua-version=luajit`
- jdk 8+,if `--enable-http-hi-java=YES`



# Installation
`./configure --help` or see `install_demo.sh` or 

```shell
                --enable-http-hi-cpp=YES                           \
                --enable-http-hi-python=NO                         \
                --enable-http-hi-lua=NO                            \
                --enable-http-hi-qjs=YES                            \
                --enable-http-hi-java=NO                           \
                --with-http-hi-python-version=python3              \
                --with-http-hi-lua-version=lua5.3                  \
                --add-module=module/ngx_http_hi_module             \
                --add-module=module/ngx_autoblacklist_module               

```


# python, lua api
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
- has_cache
- get_cache
## hi_res
- status
- content
- header
- session
- cache

# quickjs api
## hi
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
- has_cache
- get_cache
- status
- content
- header
- session
- cache


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

import hi.servlet;
import hi.request;
import hi.response;

public class jhello implements hi.servlet {

    public jhello() {

    }

    public void handler(hi.request req, hi.response res) {
        res.status = 200;
        res.content = "hello,world";

    }
}

```


## quickjs

```javascript
import * as hi from "hi";
hi.header('Content-Type','text/plain;charset=UTF-8')
hi.content('hello,world')
hi.status(200)

```

## python
```python

hi_res.status(200)
hi_res.content("hello,world")

```
### example
[pyexample](https://github.com/webcpp/pyexample)

## lua
```lua
hi_res:status(200)
hi_res:content("hello,world")

```

# 3rd party module

- nginx-push-stream-module
- ngx_http_concat_module
- ngx_http_footer_filter_module
- ngx_http_trim_filter_module
- ngx_slab_stat
- [3rd](https://www.nginx.com/resources/wiki/modules/)

# ngx_http_hi_module directives

- directives : content: loc,if in loc
    - **hi**,default: ""

    example:
    
```nginx

            location = /hello {
                hi cpp/hello.so ;
            }

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_tokens**,default: on

    example:

```nginx

        hi_need_tokens on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_cache_method**,default: GET

    example:

```nginx

        hi_cache_method GET|POST|PUT|HEAD;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_cache**,default: on

    example:

```nginx

        hi_need_cache on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_cache_size**,default: 10

    example:

```nginx

        hi_cache_size 10;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_cache_expires**,default: 300s

    example:

```nginx

        hi_cache_expires 300s;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_kvdb**,default: off

    example:

```nginx

        hi_need_kvdb on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_kvdb_size**,default: 10

    example:

```nginx

        hi_kvdb_size 10;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_kvdb_expires**,default: 300s

    example:

```nginx

        hi_kvdb_expires 300s;

```



- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_headers**,default: off

        example:

```nginx

        hi_need_headers on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_cookies**,default: off

        example:

```nginx

        hi_need_cookies on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_need_session**,default: off

        example:

```nginx

        hi_need_session on|off;

```

- directives : content: http,srv,loc,if in loc ,if in srv
    - **hi_session_expires**,default: 300s

    example:

```nginx

        hi_session_expires 300s;

```
     

- directives : content: loc,if in loc
    - **hi_python_content**,default: ""

    example:
    
```nginx

            location = /pyecho {
                hi_python_content "hi_res.status(200)\nhi_res.content('hello,world')" ;
            }

```

- directives : content: loc,if in loc
    - **hi_python_script**,default: ""

    example:
    
```nginx

            location ~ \.py$  {
                hi_python_script python;
            }

```

or

```nginx

            location / {
                hi_python_script python/index.py;
            }

```


- directives : content:  http,srv,if in srv
    - **hi_lua_package_path**,default: ""

    example:
    
```nginx


            hi_lua_package_path '/usr/local/nginx/lua/?.lua;';


```

- directives : content:  http,srv,if in srv
    - **hi_lua_package_cpath**,default: ""

    example:
    
```nginx


            hi_lua_package_cpath '/usr/local/nginx/lua/?.so;';


```



- directives : content: loc,if in loc
    - **hi_lua_content**,default: ""

    example:
    
```nginx

            location = /luaecho {
                hi_lua_content "hi_res:status(200)\nhi_res:content('hello,world')" ;
            }

```

- directives : content: loc,if in loc
    - **hi_lua_script**,default: ""

    example:
    
```nginx

            location ~ \.lua$  {
                hi_lua_script lua;
            }

```

or

```nginx

            location / {
                hi_lua_script lua/index.lua;
            }

```

- directives : content: http,srv,if in srv
    - **hi_java_classpath**,default:"-Djava.class.path=."

    example:

```nginx

hi_java_classpath "-Djava.class.path=.:/usr/local/nginx/java:/usr/local/nginx/java/hi-nginx-java.jar";


```

- directives : content: http,srv,if in srv
    - **hi_java_options**,default:"-server -d64 -Xmx1G -Xms1G -Xmn256m"

    example:

```nginx

hi_java_options "-server -d64 -Xmx3G -Xms3G -Xmn768m -XX:+DisableExplicitGC -XX:+UseConcMarkSweepGC -XX:+UseParNewGC -XX:+UseNUMA -XX:+CMSParallelRemarkEnabled -XX:MaxTenuringThreshold=15 -XX:MaxGCPauseMillis=30 -XX:GCPauseIntervalMillis=150 -XX:+UseAdaptiveGCBoundary -XX:-UseGCOverheadLimit -XX:+UseBiasedLocking -XX:SurvivorRatio=8 -XX:TargetSurvivorRatio=90 -XX:MaxTenuringThreshold=15 -Dfml.ignorePatchDiscrepancies=true -Dfml.ignoreInvalidMinecraftCertificates=true -XX:+UseFastAccessorMethods -XX:+UseCompressedOops -XX:+OptimizeStringConcat -XX:+AggressiveOpts -XX:ReservedCodeCacheSize=2048m -XX:+UseCodeCacheFlushing -XX:SoftRefLRUPolicyMSPerMB=10000 -XX:ParallelGCThreads=10";


```

- directives : content: loc,if in loc
    - **hi_java_servlet**,default:""

    example:

```nginx

hi_java_servlet hi/jhello;



```

- directives : content : http,srv,loc,if in loc ,if in srv
    - **hi_java_servlet_cache_expires**,default:300s

    example:

```nginx

hi_java_servlet_cache_expires 300s;


```

- directives : content : http,srv,loc,if in loc ,if in srv
    - **hi_java_servlet_cache_size**,default:10

    example:

```nginx

hi_java_servlet_cache_size 10;


```

- directives : content: http,srv,if in srv
    - hi_java_version,default:8

    example:

```nginx

hi_java_version 8;


```


- directives : content: loc,if in loc
    - **hi_qjs_script**,default: ""

    example:
    
```nginx
            location ~ \.js$  {
                hi_qjs_script qjs;
            }
```
or

```nginx
            location / {
                hi_qjs_script qjs/index.js;
            }
```


- directives : content:  http,srv,if in srv
    - **hi_qjs_memory_limit** ,default: 1024m

    example:
    
```nginx


            hi_qjs_memory_limit 1024m;


```


- directives : content:  http,srv,if in srv
    - **hi_qjs_stack_limit** ,default: 8m

    example:
    
```nginx


            hi_qjs_stack_limit 8m;


```

- directives : content:  http,srv,if in srv
    - **hi_qjs_ctx_called_limit**,default: 10240

    example:
    
```nginx


            hi_qjs_ctx_called_limit 10240;


```

- directives : content: loc if in loc
    - **hi_subrequest**, default : ""

    example:

```nginx

        location ^~ /sub {
                hi_subrequest '/query';
                hi_lua_content 'hi_res:header("Content-Type",hi_req:get_form("__subrequest_content_type__"))\nhi_res:status(tonumber(hi_req:get_form("__subrequest_status__")))\nhi_res:content(hi_req:get_form("__subrequest_body__"))';
        }

        location ^~ /query {
                internal;
                proxy_pass http://http://hq.sinajs.cn/;
                proxy_set_header Accept-Encoding '';
        }
#  curl -i http://localhost/sub?list=sh601006

```

# ngx_http_autoblacklist_module directives

- directives : content: http,srv,loc,if in loc ,if in srv
    - **autoblacklist**, default : "off"
    - **autoblacklist_size**, default : 1024
    - **autoblacklist_limit**, default : 30
    - **autoblacklist_expires**, default : 86400s 

    example:

```nginx

	autoblacklist on|off;
	autoblacklist_size 1024;
	autoblacklist_expires 86400s;
	autoblacklist_limit 30;

```

## nginx.conf

[hi_demo_conf](https://github.com/webcpp/hi_demo/blob/master/demo.conf)
