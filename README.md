# Introduction
A distribution of Nginx with c++ web development. 

[hi_demo](https://github.com/webcpp/hi_demo).

# Dependency
- gcc,g++(c++11)

# Installation
see `install_demo.sh` or `--add-module=ngx_http_hi_module`


# Features
- All features of nginx(latest release) are inherited, i.e., it is 100% compatible with nginx.

# Directives
- directives : content: loc
    - hi 

    example:
    
```
            location /hello {
                hi hi/hello.so ;
            }
```

- directives : content: main,srv,loc
    - hi_cache_size

        example:

```
        hi_cache_size 10;
```

- directives : content: main,srv,loc
    - hi_cache_expires

        example:

```
        hi_cache_expires 300s;
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

```
        location = /hello {
            hi hi/hello.so;
        }

```