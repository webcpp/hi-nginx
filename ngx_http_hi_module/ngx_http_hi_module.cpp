#include "module_config.hpp"
#include "cpp_handler.hpp"

static char *ngx_http_hi_conf_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);
static void ngx_http_hi_exit_process(ngx_cycle_t* cycle);
static void ngx_http_hi_exit_master(ngx_cycle_t* cycle);
static ngx_int_t ngx_http_hi_normal_handler(ngx_http_request_t *r);
static void ngx_http_hi_body_handler(ngx_http_request_t* r);
static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r);


ngx_http_module_t ngx_http_hi_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */
    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_hi_create_loc_conf, /* create location configuration */
    ngx_http_hi_merge_loc_conf /* merge location configuration */
};

ngx_command_t ngx_http_hi_commands[] = {
    {
        ngx_string("hi"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, module_path),
        NULL
    },
    {
        ngx_string("hi_cache_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, cache_size),
        NULL
    },
    {
        ngx_string("hi_cache_expires"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, cache_expires),
        NULL
    },
    {
        ngx_string("hi_kvdb_expires"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, kvdb_expires),
        NULL
    },
    {
        ngx_string("hi_kvdb_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, kvdb_size),
        NULL
    },
    {
        ngx_string("hi_need_headers"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_headers),
        NULL
    },
    {
        ngx_string("hi_need_cache"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_cache),
        NULL
    },
    {
        ngx_string("hi_need_kvdb"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_kvdb),
        NULL
    },
    {
        ngx_string("hi_need_cookies"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_cookies),
        NULL
    },
    {
        ngx_string("hi_redis_host"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, redis_host),
        NULL
    },
    {
        ngx_string("hi_redis_port"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, redis_port),
        NULL
    },
    {
        ngx_string("hi_need_session"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, need_session),
        NULL
    },
    {
        ngx_string("hi_session_expires"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, session_expires),
        NULL
    },
#ifdef HTTP_HI_PYTHON
    {
        ngx_string("hi_python_script"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, python_script),
        NULL
    },
    {
        ngx_string("hi_python_content"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, python_content),
        NULL
    },
#endif
#ifdef HTTP_HI_LUA
    {
        ngx_string("hi_lua_script"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, lua_script),
        NULL
    },
    {
        ngx_string("hi_lua_content"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, lua_content),
        NULL
    },
    {
        ngx_string("hi_lua_package_path"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, lua_package_path),
        NULL
    },
    {
        ngx_string("hi_lua_package_cpath"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, lua_package_cpath),
        NULL
    },
#endif
#ifdef HTTP_HI_JAVA
    {
        ngx_string("hi_java_classpath"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_classpath),
        NULL
    },
    {
        ngx_string("hi_java_options"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_options),
        NULL
    },
    {
        ngx_string("hi_java_servlet"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_servlet),
        NULL
    },
    {
        ngx_string("hi_java_servlet_cache_expires"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_servlet_cache_expires),
        NULL
    },
    {
        ngx_string("hi_java_servlet_cache_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_servlet_cache_size),
        NULL
    },
    {
        ngx_string("hi_java_version"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, java_version),
        NULL
    },
    {
        ngx_string("hi_javascript_script"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, javascript_script),
        NULL
    },
    {
        ngx_string("hi_javascript_content"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, javascript_content),
        NULL
    },
    {
        ngx_string("hi_javascript_lang"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, javascript_lang),
        NULL
    },
    {
        ngx_string("hi_javascript_extension"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, javascript_extension),
        NULL
    },
    {
        ngx_string("hi_javascript_compiledscript_expires"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_sec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, javascript_compiledscript_expires),
        NULL
    },
#endif
#ifdef HTTP_HI_PHP
    {
        ngx_string("hi_php_script"),
        NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
        ngx_http_hi_conf_init,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_hi_loc_conf_t, php_script),
        NULL
    },
#endif
    ngx_null_command
};

ngx_module_t ngx_http_hi_module = {
    NGX_MODULE_V1,
    &ngx_http_hi_module_ctx, /* module context */
    ngx_http_hi_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    ngx_http_hi_exit_process, /* exit process */
    ngx_http_hi_exit_master, /* exit master */
    NGX_MODULE_V1_PADDING
};

static char *ngx_http_hi_conf_init(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    if (mtx == 0) {
        mtx = (pthread_mutex_t*) mmap(0, sizeof (pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mtx != MAP_FAILED) {
            if (mtx_attr == 0) {
                mtx_attr = (pthread_mutexattr_t*) mmap(0, sizeof (pthread_mutexattr_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                if (mtx_attr != MAP_FAILED) {
                    pthread_mutexattr_init(mtx_attr);
                    pthread_mutexattr_setpshared(mtx_attr, PTHREAD_PROCESS_SHARED);
                    pthread_mutexattr_settype(mtx_attr, PTHREAD_MUTEX_DEFAULT);
                    pthread_mutex_init(mtx, mtx_attr);
                    if (cpu_count == 0) {
                        cpu_count = (size_t*) mmap(0, sizeof (size_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                        if (cpu_count != MAP_FAILED) {
                            pthread_mutex_lock(mtx);
                            *cpu_count = 0;
                            pthread_mutex_unlock(mtx);
                        }
                    }

                }
            }
        }
    }
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *) ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_hi_handler;
    ngx_conf_set_str_slot(cf, cmd, conf);
    return NGX_CONF_OK;
}

static void * ngx_http_hi_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_hi_loc_conf_t *conf = (ngx_http_hi_loc_conf_t*) ngx_pcalloc(cf->pool, sizeof (ngx_http_hi_loc_conf_t));
    if (conf) {
        conf->module_path.len = 0;
        conf->module_path.data = NULL;
        conf->module_index = NGX_CONF_UNSET;

        conf->redis_host.len = 0;
        conf->redis_host.data = NULL;
        conf->redis_port = NGX_CONF_UNSET;

        conf->cache_index = NGX_CONF_UNSET;
        conf->cache_size = NGX_CONF_UNSET_UINT;
        conf->cache_expires = NGX_CONF_UNSET;


        conf->session_expires = NGX_CONF_UNSET;

        conf->kvdb_size = NGX_CONF_UNSET_UINT;
        conf->kvdb_expires = NGX_CONF_UNSET;

        conf->need_headers = NGX_CONF_UNSET;
        conf->need_cache = NGX_CONF_UNSET;
        conf->need_cookies = NGX_CONF_UNSET;
        conf->need_session = NGX_CONF_UNSET;
        conf->need_kvdb = NGX_CONF_UNSET;

        conf->app_type = application_t::__unkown__;
#ifdef HTTP_HI_PYTHON
        conf->python_script.len = 0;
        conf->python_script.data = NULL;
        conf->python_content.len = 0;
        conf->python_content.data = NULL;
#endif
#ifdef HTTP_HI_LUA
        conf->lua_script.len = 0;
        conf->lua_script.data = NULL;
        conf->lua_content.len = 0;
        conf->lua_content.data = NULL;
        conf->lua_package_path.len = 0;
        conf->lua_package_path.data = NULL;
        conf->lua_package_cpath.len = 0;
        conf->lua_package_cpath.data = NULL;
#endif
#ifdef HTTP_HI_PHP
        conf->php_script.len = 0;
        conf->php_script.data = NULL;
#endif
#ifdef HTTP_HI_JAVA
        conf->java_classpath.len = 0;
        conf->java_classpath.data = NULL;
        conf->java_servlet.len = 0;
        conf->java_servlet.data = NULL;
        conf->java_servlet_cache_size = NGX_CONF_UNSET_UINT;
        conf->java_servlet_cache_expires = NGX_CONF_UNSET;
        conf->java_version = NGX_CONF_UNSET;
        conf->javascript_content.data = NULL;
        conf->javascript_content.len = 0;
        conf->javascript_script.data = NULL;
        conf->javascript_script.len = 0;
        conf->javascript_lang.data = NULL;
        conf->javascript_lang.len = 0;
        conf->javascript_extension.data = NULL;
        conf->javascript_extension.len = 0;
        conf->javascript_engine_index = NGX_CONF_UNSET;
        conf->javascript_compiledscript_expires = NGX_CONF_UNSET;
#endif
        return conf;
    }
    return NGX_CONF_ERROR;
}

static char * ngx_http_hi_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
    ngx_http_hi_loc_conf_t * prev = (ngx_http_hi_loc_conf_t*) parent;
    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t*) child;

    ngx_conf_merge_str_value(conf->module_path, prev->module_path, "");
    ngx_conf_merge_str_value(conf->redis_host, prev->redis_host, "");
    ngx_conf_merge_value(conf->redis_port, prev->redis_port, (ngx_int_t) 0);

#ifdef HTTP_HI_PYTHON
    ngx_conf_merge_str_value(conf->python_script, prev->python_script, "");
    ngx_conf_merge_str_value(conf->python_content, prev->python_content, "");
#endif
#ifdef HTTP_HI_LUA
    ngx_conf_merge_str_value(conf->lua_script, prev->lua_script, "");
    ngx_conf_merge_str_value(conf->lua_content, prev->lua_content, "");
    ngx_conf_merge_str_value(conf->lua_package_path, prev->lua_package_path, "");
    ngx_conf_merge_str_value(conf->lua_package_cpath, prev->lua_package_cpath, "");
#endif
#ifdef HTTP_HI_PHP
    ngx_conf_merge_str_value(conf->php_script, prev->php_script, "");
#endif
#ifdef HTTP_HI_JAVA
    ngx_conf_merge_str_value(conf->java_classpath, prev->java_classpath, "-Djava.class.path=.");
    ngx_conf_merge_str_value(conf->java_options, prev->java_options, "-server -d64 -Xmx1G -Xms1G -Xmn256m -Dnashorn.args=--global-per-engine");
    ngx_conf_merge_str_value(conf->java_servlet, prev->java_servlet, "");
    ngx_conf_merge_uint_value(conf->java_servlet_cache_size, prev->java_servlet_cache_size, (size_t) 10);
    ngx_conf_merge_sec_value(conf->java_servlet_cache_expires, prev->java_servlet_cache_expires, (ngx_int_t) 300);

    ngx_conf_merge_str_value(conf->javascript_script, prev->javascript_script, "");
    ngx_conf_merge_str_value(conf->javascript_content, prev->javascript_content, "");
    ngx_conf_merge_str_value(conf->javascript_lang, prev->javascript_lang, "javascript");
    ngx_conf_merge_str_value(conf->javascript_extension, prev->javascript_extension, "js");
    ngx_conf_merge_sec_value(conf->javascript_compiledscript_expires, prev->javascript_compiledscript_expires, (ngx_int_t) 300);
#ifdef JNI_VERSION_9
    ngx_conf_merge_value(conf->java_version, prev->java_version, (ngx_int_t) 9);
#else
    ngx_conf_merge_value(conf->java_version, prev->java_version, (ngx_int_t) 8);
#endif
#endif

    ngx_conf_merge_uint_value(conf->cache_size, prev->cache_size, (size_t) 10);
    ngx_conf_merge_sec_value(conf->cache_expires, prev->cache_expires, (ngx_int_t) 300);
    ngx_conf_merge_uint_value(conf->kvdb_size, prev->kvdb_size, (size_t) 10);
    ngx_conf_merge_sec_value(conf->kvdb_expires, prev->kvdb_expires, (ngx_int_t) 300);
    ngx_conf_merge_sec_value(conf->session_expires, prev->session_expires, (ngx_int_t) 300);
    ngx_conf_merge_value(conf->need_headers, prev->need_headers, (ngx_flag_t) 0);
    ngx_conf_merge_value(conf->need_cache, prev->need_cache, (ngx_flag_t) 0);
    ngx_conf_merge_value(conf->need_kvdb, prev->need_kvdb, (ngx_flag_t) 1);
    ngx_conf_merge_value(conf->need_cookies, prev->need_cookies, (ngx_flag_t) 0);
    ngx_conf_merge_value(conf->need_session, prev->need_session, (ngx_flag_t) 0);

    if (conf->need_cache == 1 && conf->cache_index == NGX_CONF_UNSET) {
        CACHE.push_back(std::move(std::make_shared<lru11::Cache<std::string, std::shared_ptr < hi::cache_t>>>(conf->cache_size)));
        conf->cache_index = CACHE.size() - 1;
    }

    if (conf->need_session == 1 && conf->need_cookies == 0) {
        conf->need_cookies = 1;
    }
    if (conf->module_path.len > 0 && conf->module_index == NGX_CONF_UNSET) {
        PLUGIN.push_back(std::move(std::make_shared<hi::module < hi::servlet >> ((char*) conf->module_path.data)));
        conf->module_index = PLUGIN.size() - 1;
        conf->app_type = application_t::__cpp__;
    }
#ifdef HTTP_HI_PYTHON
    if (conf->python_content.len > 0 || conf->python_script.len > 0) {
        conf->app_type = application_t::__python__;
    }
#endif
#ifdef HTTP_HI_LUA
    if (conf->lua_content.len > 0 || conf->lua_script.len > 0) {
        conf->app_type = application_t::__lua__;
    }
#endif
#ifdef HTTP_HI_PHP
    if (conf->php_script.len > 0) {
        conf->app_type = application_t::__php__;
    }
#endif
#ifdef HTTP_HI_JAVA
    if (conf->java_servlet.len > 0) {
        conf->app_type = application_t::__java__;
        if (!JAVA_SERVLET_CACHE) {
            JAVA_SERVLET_CACHE = std::move(std::make_shared<hi::cache::lru_cache < std::string, hi::java_servlet_t >> (conf->java_servlet_cache_size));
        }
    }

    if (conf->javascript_content.len > 0 || conf->javascript_script.len > 0) {
        conf->app_type = application_t::__javascript__;
    }

#endif


    return NGX_CONF_OK;
}

static void ngx_http_hi_exit_process(ngx_cycle_t * cycle) {
    if (mtx)pthread_mutex_destroy(mtx);
    if (mtx_attr)pthread_mutexattr_destroy(mtx_attr);
    if (mtx_attr)munmap(mtx_attr, sizeof (pthread_mutexattr_t));
    if (mtx)munmap(mtx, sizeof (pthread_mutex_t));
    if (cpu_count)munmap(cpu_count, sizeof (size_t));
    PLUGIN.clear();
    CACHE.clear();
    if (LEVELDB) {
        delete LEVELDB;
    }
#ifdef HTTP_HI_PYTHON
    PYTHON.reset();
#endif
#ifdef HTTP_HI_LUA
    LUA.reset();
#endif
#ifdef HTTP_HI_JAVA
    JAVA.reset();
    JAVA_SERVLET_CACHE.reset();
    hi::java::JAVA_IS_READY = false;
#endif
#ifdef HTTP_HI_PHP
    PHP.reset();
#endif
}

static void ngx_http_hi_exit_master(ngx_cycle_t * cycle) {

}

static ngx_int_t ngx_http_hi_normal_handler(ngx_http_request_t *r) {

    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_hi_module);

    if (conf->need_cache == 1 && r->headers_in.if_modified_since && r->headers_in.if_modified_since->value.data) {
        time_t now = time(NULL), old = ngx_http_parse_time(r->headers_in.if_modified_since->value.data, r->headers_in.if_modified_since->value.len);
        if (difftime(now, old) <= conf->cache_expires) {
            return NGX_HTTP_NOT_MODIFIED;
        }
    }

    hi::request ngx_request;
    hi::response ngx_response;
    std::string SESSION_ID_VALUE;
    std::unordered_map<std::string, std::string>::const_iterator iterator;


    ngx_request.uri.assign((char*) r->uri.data, r->uri.len);
    if (r->args.len > 0) {
        ngx_request.param.assign((char*) r->args.data, r->args.len);
    }
    std::shared_ptr<std::string> cache_k = std::make_shared<std::string>(ngx_request.uri);
    if (r->args.len > 0) {
        cache_k->append("?").append(ngx_request.param);
    }
    cache_k->assign(std::move(hi::md5(*cache_k)));
    if (r->method == NGX_HTTP_GET && conf->need_cache == 1) {
        auto lru_cache = CACHE[conf->cache_index];
        if (lru_cache->contains(*cache_k)) {
            auto cache_ele = lru_cache->get(*cache_k);
            if (cache_ele->expired(conf->cache_expires)) {
                lru_cache->remove(*cache_k);
            } else {
                ngx_response.content = cache_ele->content;
                ngx_response.headers.find("Content-Type")->second = cache_ele->content_type;
                ngx_response.status = cache_ele->status;
                u_char tmp_t[sizeof ("Mon, 28 Sep 1970 06:00:00 GMT") - 1];
                u_char* p = ngx_http_time(tmp_t, time(0));
                ngx_response.headers.insert(std::move(std::make_pair("Last-Modified", std::string((char*) tmp_t, p - tmp_t))));
                goto done;
            }
        }
    }


    if (conf->need_headers == 1) {
        hi::get_input_headers(r, ngx_request.headers);
    }

    ngx_request.method.assign((char*) r->method_name.data, r->method_name.len);
    ngx_request.client.assign((char*) r->connection->addr_text.data, r->connection->addr_text.len);
    if (r->headers_in.user_agent->value.len > 0) {
        ngx_request.user_agent.assign((char*) r->headers_in.user_agent->value.data, r->headers_in.user_agent->value.len);
    }
    if (r->args.len > 0) {
        hi::parser_param(ngx_request.param, ngx_request.form);
    }
    if (r->headers_in.content_length_n > 0) {
        ngx_str_t body = hi::get_input_body(r);
        if (ngx_strncasecmp(r->headers_in.content_type->value.data, (u_char *) form_multipart_type,
                form_multipart_type_len) == 0) {
            ngx_http_core_loc_conf_t *clcf = (ngx_http_core_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_core_module);
            std::string upload_err_msg;
            if (!hi::upload(ngx_request, &body, clcf, r, TEMP_DIRECTORY, upload_err_msg)) {
                ngx_response.content = std::move(upload_err_msg);
                ngx_response.status = 500;
                goto done;
            }
        } else if (ngx_strncasecmp(r->headers_in.content_type->value.data, (u_char *) form_urlencoded_type,
                form_urlencoded_type_len) == 0) {
            hi::parser_param(std::string((char*) body.data, body.len), ngx_request.form);
        } else {
            ngx_request.form["__body__"] = std::move(std::string((char*) body.data, body.len));
        }
    }
    if (conf->need_cookies == 1 && r->headers_in.cookies.elts != NULL && r->headers_in.cookies.nelts != 0) {
        ngx_table_elt_t ** cookies = (ngx_table_elt_t **) r->headers_in.cookies.elts;
        for (size_t i = 0; i < r->headers_in.cookies.nelts; ++i) {
            if (cookies[i]->value.data != NULL) {
                hi::parser_param(std::string((char*) cookies[i]->value.data, cookies[i]->value.len), ngx_request.cookies, ';');
            }
        }
    }

    if (conf->need_session == 1 && (iterator = ngx_request.cookies.find(SESSION_ID_NAME)) != ngx_request.cookies.end()) {
        if (LEVELDB) {
            SESSION_ID_VALUE = iterator->second;
            std::string v;
            if (LEVELDB->Get(leveldb::ReadOptions(), SESSION_ID_VALUE, &v).ok()) {
                hi::deserialize(v, ngx_request.session);
            } else {
                LEVELDB->Put(leveldb::WriteOptions(), SESSION_ID_VALUE, hi::serialize(ngx_request.session));
            }
        }
    }


    if (conf->need_kvdb == 1) {
        if (LEVELDB) {
            std::string cache_v;
            if (LEVELDB->Get(leveldb::ReadOptions(), *cache_k, &cache_v).ok()) {
                hi::deserialize(cache_v, ngx_request.cache);
            } else {
                LEVELDB->Put(leveldb::WriteOptions(), *cache_k, hi::serialize(ngx_request.cache));
            }
        }
    }

    switch (conf->app_type) {
        case application_t::__cpp__:hi::ngx_http_hi_cpp_handler(conf, ngx_request, ngx_response);
            break;
#ifdef HTTP_HI_PYTHON
        case application_t::__python__:hi::ngx_http_hi_python_handler(conf, ngx_request, ngx_response);
            break;
#endif
#ifdef HTTP_HI_LUA
        case application_t::__lua__:hi::ngx_http_hi_lua_handler(conf, ngx_request, ngx_response);
            break;
#endif
#ifdef HTTP_HI_JAVA
        case application_t::__java__:hi::ngx_http_hi_java_handler(conf, ngx_request, ngx_response);
            break;
        case application_t::__javascript__:hi::ngx_http_hi_javascript_handler(conf, ngx_request, ngx_response);
            break;
#endif
#ifdef HTTP_HI_PHP
        case application_t::__php__:hi::ngx_http_hi_php_handler(conf, ngx_request, ngx_response);
            break;
#endif
        default:break;
    }

    if (r->method == NGX_HTTP_GET && conf->need_cache == 1 && ngx_response.status == 200 && conf->cache_expires > 0) {
        std::shared_ptr<hi::cache_t> cache_v = std::make_shared<hi::cache_t>();
        cache_v->content = ngx_response.content;
        cache_v->content_type = ngx_response.headers.find("Content-Type")->second;
        CACHE[conf->cache_index]->insert(*cache_k, cache_v);
        u_char tmp_t[sizeof ("Mon, 28 Sep 1970 06:00:00 GMT") - 1];
        u_char* p = ngx_http_time(tmp_t, time(0));
        ngx_response.headers.insert(std::move(std::make_pair("Last-Modified", std::string((char*) tmp_t, p - tmp_t))));
    }

    if (conf->need_session == 1 && LEVELDB && !ngx_response.session.empty()) {
        std::unordered_map<std::string, std::string>* ptr = 0;
        if (!ngx_request.session.empty()) {
            for (auto &i : ngx_response.session) {
                ngx_request.session[i.first] = std::move(i.second);
            }
            ptr = &ngx_request.session;
        } else {
            ptr = &ngx_response.session;
        }
        LEVELDB->Put(leveldb::WriteOptions(), SESSION_ID_VALUE, hi::serialize(*ptr));
    }

    if (conf->need_kvdb == 1 && LEVELDB && !ngx_response.cache.empty()) {
        std::unordered_map<std::string, std::string>* ptr = 0;
        if (!ngx_request.cache.empty()) {
            for (auto &i : ngx_response.cache) {
                ngx_request.cache[i.first] = std::move(i.second);
            }
            ptr = &ngx_request.cache;
        } else {
            ptr = &ngx_response.cache;
        }
        LEVELDB->Put(leveldb::WriteOptions(), *cache_k, hi::serialize(*ptr));
    }

done:
    ngx_str_t response;
    response.data = (u_char*) ngx_response.content.c_str();
    response.len = ngx_response.content.size();

    if (response.len == 0) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to response size.");
        return NGX_HTTP_NOT_FOUND;
    }

    ngx_buf_t *buf;
    buf = (ngx_buf_t*) ngx_pcalloc(r->pool, sizeof (ngx_buf_t));
    if (buf == NULL) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "Failed to allocate response buffer.");
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    buf->pos = response.data;
    buf->last = buf->pos + response.len;
    buf->memory = 1;
    buf->last_buf = 1;

    ngx_chain_t out;
    out.buf = buf;
    out.next = NULL;

    ngx_response.headers.insert(std::make_pair(HI_NGINX_SERVER_NAME, HI_NGINX_SERVER_VERSION));
    hi::set_output_headers(r, ngx_response.headers);
    r->headers_out.status = ngx_response.status;
    r->headers_out.content_length_n = response.len;

    ngx_int_t rc;
    rc = ngx_http_send_header(r);
    if (rc != NGX_OK) {

        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }
    return ngx_http_output_filter(r, &out);
}

static void ngx_http_hi_body_handler(ngx_http_request_t* r) {
    ngx_http_finalize_request(r, ngx_http_hi_normal_handler(r));
}

static ngx_int_t ngx_http_hi_handler(ngx_http_request_t *r) {
    ngx_http_hi_loc_conf_t * conf = (ngx_http_hi_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_hi_module);
    if (!LEVELDB) {
        LEVELDB_OPTIONS.create_if_missing = true;
        std::string i;
        pthread_mutex_lock(mtx);
        if (*cpu_count > std::thread::hardware_concurrency() - 1) {
            *cpu_count = 0;
        }
        i = std::move(std::to_string(*cpu_count));
        *cpu_count = (*cpu_count) + 1;
        pthread_mutex_unlock(mtx);
        leveldb::DB::Open(LEVELDB_OPTIONS, LEVELDB_PATH + ("/" + i), &LEVELDB);
    }


    if (r->headers_in.content_length_n > 0) {
        ngx_http_core_loc_conf_t *clcf = (ngx_http_core_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_core_module);
        if (clcf->client_body_buffer_size < (size_t) clcf->client_max_body_size) {
            clcf->client_body_buffer_size = clcf->client_max_body_size;
        }
        r->request_body_in_single_buf = 1;
        r->request_body_file_log_level = 0;
        ngx_int_t rc = ngx_http_read_client_request_body(r, ngx_http_hi_body_handler);
        if (rc >= NGX_HTTP_SPECIAL_RESPONSE) {
            return rc;
        }
        return NGX_DONE;
    } else {
        ngx_http_discard_request_body(r);
        return ngx_http_hi_normal_handler(r);
    }
}
