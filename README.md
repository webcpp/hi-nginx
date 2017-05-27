# Introduction
A distribution of Nginx with c++ web development. 

[hi_demo](https://github.com/webcpp/hi_demo).

# Dependency
- linux
- gcc,g++(c++11)
- hiredis-devel

# Installation
see `install_demo.sh` or `--add-module=ngx_http_hi_module`


# Features
- All features of nginx(latest release) are inherited, i.e., it is 100% compatible with nginx.

# Directives
- directives : content: loc,if in loc
    - hi,default: ""

    example:
    
```
            location /hello {
                hi hi/hello.so ;
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

# hello,world

## class

```
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

## compile

```
g++ -std=c++11 -I/home/centos7/nginx/include  -shared -fPIC hello.cpp -o hello.so
install hello.so /home/centos7/nginx/hi

```


## nginx.conf

[hi_demo_conf](https://github.com/webcpp/hi_demo/blob/master/demo.conf)