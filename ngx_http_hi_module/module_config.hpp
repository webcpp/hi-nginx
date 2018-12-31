#ifndef MODULE_CONFIG_HPP
#define MODULE_CONFIG_HPP

extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_md5.h>
#include <ngx_conf_file.h>
#include <ngx_http_variables.h>
}

#define HI_NGINX_SERVER_VERSION "1.8.6"
#define HI_NGINX_SERVER_NAME "hi-nginx/" HI_NGINX_SERVER_VERSION
#define HI_NGINX_SERVER_HEAD "PoweredBy"
#define SESSION_ID_NAME "SESSIONID"
#define form_multipart_type "multipart/form-data"
#define form_multipart_type_len (sizeof(form_multipart_type) - 1)
#define form_urlencoded_type "application/x-www-form-urlencoded"
#define form_urlencoded_type_len (sizeof(form_urlencoded_type) - 1)
#define TEMP_DIRECTORY "temp"
#define LEVELDB_PATH "leveldb"

#define REQUEST_BODY_NAME "__body__"
#define SUBREQUEST_BODY_NAME "__subrequest_body__"
#define SUBREQUEST_STATUS_NAME "__subrequest_status__"
#define SUBREQUEST_CONTENT_TYPE_NAME "__subrequest_content_type__"

#define HTTP_TIME_SIZE (sizeof ("Mon, 28 Sep 1970 06:00:00 GMT")-1)

//#define HTTP_HI_LUA
//#define HTTP_HI_JAVA
//#define HTTP_HI_PYTHON
//#define HTTP_HI_PHP
//#define HTTP_HI_DUKTAPE

#ifndef HTTP_HI_CPP
#define HTTP_HI_CPP
#endif

#include <sys/mman.h>
#include <pthread.h>
#include <vector>
#include <memory>
#include <utility>
#include <fstream>
#include <streambuf>
#include <exception>
#include <queue>


#include "include/request.hpp"
#include "include/response.hpp"
#include "include/servlet.hpp"
#include "lib/module.hpp"
#include "lib/LRUCache11.hpp"
#include "lib/lrucache.hpp"
#include "lib/param.hpp"
#include "lib/MPFDParser/Parser.h"
#include "lib/leveldb/db.h"
#include "lib/msgpack/msgpack.hpp"

#include "cache_t.hpp"
#include "application_t.hpp"
#include "utils.hpp"

static pthread_mutex_t *mtx = 0;
static pthread_mutexattr_t *mtx_attr = 0;
static size_t *cpu_count = 0;
static std::vector<std::shared_ptr<hi::module<hi::servlet>>> PLUGIN;
static std::vector<std::shared_ptr<lru11::Cache<std::string, std::shared_ptr<hi::cache_t>>>> CACHE;
static leveldb::DB* LEVELDB = 0;
static leveldb::Options LEVELDB_OPTIONS;
static std::vector<std::shared_ptr<hi::request>> SUBREQUEST_RESPONSE;
static ngx_conf_enum_t cache_method_enums[] = {
    { ngx_string("GET"), NGX_HTTP_GET},
    { ngx_string("POST"), NGX_HTTP_POST},
    { ngx_string("PUT"), NGX_HTTP_PUT},
    { ngx_string("HEAD"), NGX_HTTP_HEAD},
    { ngx_null_string, 0}
};

#ifdef HTTP_HI_PYTHON
#include "lib/py_request.hpp"
#include "lib/py_response.hpp"
#include "lib/pybind11_py.hpp"
static std::shared_ptr<hi::pybind11_py> PYTHON;
#endif

#ifdef HTTP_HI_LUA
#include "lib/lua.hpp"
static std::shared_ptr<hi::lua> LUA;
#endif

#ifdef HTTP_HI_JAVA
#include "lib/java.hpp"
static std::shared_ptr<hi::java> JAVA;
static std::shared_ptr<hi::cache::lru_cache<std::string, hi::java_servlet_t>> JAVA_SERVLET_CACHE;
#endif

#ifdef HTTP_HI_PHP
#include "lib/php-x/phpx.h"
#include "lib/php-x/phpx_embed.h"
static std::shared_ptr<php::VM> PHP;
#endif

#ifdef HTTP_HI_DUKTAPE
#include "lib/duktape.hpp"
static std::shared_ptr<hi::duktape> DUKTAPE;
#endif

typedef struct {
    ngx_str_t module_path
    , redis_host
    , subrequest
#ifdef HTTP_HI_PYTHON
    , python_script
    , python_content
#endif
#ifdef HTTP_HI_LUA
    , lua_script
    , lua_content
    , lua_package_path
    , lua_package_cpath
#endif
#ifdef HTTP_HI_JAVA
    , java_classpath
    , java_options
    , java_servlet
    , javascript_script
    , javascript_content
    , javascript_lang
    , javascript_extension
#endif
#ifdef HTTP_HI_PHP
    , php_script
#endif
#ifdef HTTP_HI_DUKTAPE
    , duktape_script
    , duktape_content
    , duktape_package_path
    , duktape_package_cpath
#endif
    ;
    ngx_int_t module_index
    , subrequest_index
    , redis_port
    , cache_expires
    , session_expires
    , cache_index
    , kvdb_expires
#ifdef HTTP_HI_JAVA
    , java_servlet_cache_expires
    , java_version
    , javascript_engine_index
    , javascript_compiledscript_expires
#endif
    ;
    size_t cache_size
    , kvdb_size
#ifdef HTTP_HI_JAVA
    , java_servlet_cache_size
#endif
    ;
    ngx_flag_t need_headers
    , need_cache
    , need_cookies
    , need_session
    , need_kvdb
    , need_tokens;
    application_t app_type;
    ngx_uint_t cache_method;
} ngx_http_hi_loc_conf_t;


#endif /* MODULE_CONFIG_HPP */

