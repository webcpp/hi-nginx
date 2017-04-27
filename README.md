# Introduction
A distribution of Nginx with c++ web development. 

[hi_demo](https://github.com/webcpp/hi_demo).

# Dependency
- gcc,g++(c++11)
- Poco framework

# Installation
see `install_demo.sh` or `--add-module=ngx_http_hi_module`


# Features
- All features of nginx(latest release) are inherited, i.e., it is 100% compatible with nginx.

# Directives
- directives : content: location
    - hi 

    example:
    
```
            location /hello {
                hi hi/hello.so ;
            }
```

# hello,world

## class

```
#include "view.hpp"
namespace hi{
class hello : public view {
    public:

        void handler(request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
            res.content = "hello,world";
            res.status = 200;
        }

    };
}

extern "C" hi::view* create() {
    return new hi::hello();
}

extern "C" void destroy(hi::view* p) {
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