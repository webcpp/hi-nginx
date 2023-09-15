# Features
- All features of nginx(latest release) are inherited, i.e., it is 100% compatible with nginx.
- Web development using python, c++, lua, java

# Document

[https://github.com/webcpp/hi-nginx-tutorial](https://github.com/webcpp/hi-nginx-tutorial)

# Installation
`./configure --help` or see `install_demo.sh` or 

```shell

--add-module=module/ngx_http_autoblacklist_module   \
--with-http-hi-lua-version=lua5.3                   \
--add-module=module/ngx_http_lua_module             \
--with-http-hi-python-version=python-3.8-embed      \
--add-module=module/ngx_http_py_module              \
--add-module=module/ngx_http_java_module            \
--add-module=module/ngx_http_cpp_module    


```

## nginx.conf

```nginx

server {
    listen 80;
    server_name localhost;
	
    autoblacklist on;
    autoblacklist_size 1024;
    autoblacklist_expires 1d;
    autoblacklist_limit 30;

    location ~ \.cdp {
        cpp_load;
        cpp_search_path "/usr/local/nginx/app/cpp";
        cpp_uri_pattern ".*\.cdp$";
        cpp_expires 1m;
    }

    location ~ \.pdp {
        py_load;
        py_search_path "/usr/local/nginx/app/python";
        py_uri_pattern ".*\.pdp$";
        py_expires 1m;
    }
    
    location ~ \.ldp {
        lua_load;
        lua_package_search_path "/usr/local/nginx/app/lua/?.lua;/usr/local/nginx/app/lua/package/?.lua";
        lua_cpackage_search_path "/usr/local/nginx/app/lua/?.so;/usr/local/nginx/app/lua/cpackage/?.so";
        lua_uri_pattern ".*\.ldp$";
        lua_memory_limit 50;
        lua_expires 1m;
    }
    location ~ \.jdp {
        java_load;
        java_class_path "-Djava.class.path=.:/usr/local/nginx/app/java:/usr/local/nginx/app/java/hi-nginx-java.jar:/usr/local/nginx/app/java/mariadb-java-client-2.7.4.jar:/usr/local/nginx/app/java/mysql-connector-java-8.0.26.jar:/usr/local/nginx/app/java/druid-1.2.6.jar:/usr/local/nginx/app/java/demo.jar";
        java_options "-server -d64  -Dconfig.file=/usr/local/nginx/app/java/application.conf";
        java_servlet "hi/controller";
        java_uri_pattern ".*\.jdp$";
        java_expires 1m;
        java_version 11;    
    }

    location / {
        access_log off;
        root   html;
        index  index.html index.htm;
    }

}


```
