# Introduction
A distribution of Nginx with c++ web development. 

[hinginx demo](https://github.com/webcpp/hinginx_demo).

# Dependency
- gcc,g++(c++11)
- Poco framework

# Installation
see `init.sh`


# Features
- All features of nginx-1.11.13 are inherited, i.e., it is 100% compatible with nginx.

# Directives
- directives : context :  http, server
    - cpp_module_dir,           default ''

    example:

```
            cpp_module_dir      /home/centos7/hi/cpp;
```
- directives : content: location
    - cpp_call 

    example:
    
```
            location /hello {
                cpp_call hi::hello ;
            }
```

# hello,world

## class

```
namespace hi {

    class hello : public view {
    public:

        void handler(request& req, response& res) {
            res.headers.find("Content-Type")->second = "text/plain;charset=UTF-8";
            res.content = "hello,world";
            res.status = 200;
        }

    };

}

```
## nginx.conf

```
        cpp_module_dir /home/centos7/hi/cpp;

        location = /hello {
            cpp_call hi::hello;
        }

```