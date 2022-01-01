#include "ngx_http_autoblacklist_module.hpp"

static ngx_int_t ngx_http_autoblacklist_init(ngx_conf_t *cf);
static void *ngx_http_autoblacklist_create_conf(ngx_conf_t *cf);
static char *ngx_http_autoblacklist_merge_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_autoblacklist_handler(ngx_http_request_t *r);
static void ngx_http_autoblacklist_process_exit(ngx_cycle_t *cycle);

static ngx_command_t ngx_http_autoblacklist_commands[] = {
    {ngx_string("autoblacklist"),
     NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_flag_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_autoblacklist_loc_conf_t, enable),
     NULL},
    {ngx_string("autoblacklist_expires"),
     NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_sec_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_autoblacklist_loc_conf_t, expires),
     NULL},
    {ngx_string("autoblacklist_limit"),
     NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_num_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_autoblacklist_loc_conf_t, limit),
     NULL},
    {ngx_string("autoblacklist_size"),
     NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_SIF_CONF | NGX_HTTP_LOC_CONF | NGX_HTTP_LIF_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_num_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof(ngx_http_autoblacklist_loc_conf_t, size),
     NULL},
    ngx_null_command};

static ngx_http_module_t ngx_http_autoblacklist_module_ctx = {
    NULL,                        /* preconfiguration */
    ngx_http_autoblacklist_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_autoblacklist_create_conf, /* create location configuration */
    ngx_http_autoblacklist_merge_conf   /* merge location configuration */
};

ngx_module_t ngx_http_autoblacklist_module = {
    NGX_MODULE_V1,
    &ngx_http_autoblacklist_module_ctx,  /* module context */
    ngx_http_autoblacklist_commands,     /* module directives */
    NGX_HTTP_MODULE,                     /* module type */
    NULL,                                /* init master */
    NULL,                                /* init module */
    NULL,                                /* init process */
    NULL,                                /* init thread */
    NULL,                                /* exit thread */
    ngx_http_autoblacklist_process_exit, /* exit process */
    NULL,                                /* exit master */
    NGX_MODULE_V1_PADDING};

static ngx_int_t
ngx_http_autoblacklist_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt *h;
    ngx_http_core_main_conf_t *cmcf;

    cmcf = (ngx_http_core_main_conf_t *)ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = (ngx_http_handler_pt *)ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
    if (h == NULL)
    {
        return NGX_ERROR;
    }

    *h = ngx_http_autoblacklist_handler;

    return NGX_OK;
}

static void ngx_http_autoblacklist_process_exit(ngx_cycle_t *cycle)
{
    if (BLACKLIST)
    {
        BLACKLIST.reset();
    }
}

static void *ngx_http_autoblacklist_create_conf(ngx_conf_t *cf)
{
    ngx_http_autoblacklist_loc_conf_t *conf = (ngx_http_autoblacklist_loc_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_autoblacklist_loc_conf_t));
    if (conf)
    {
        conf->enable = NGX_CONF_UNSET;
        conf->expires = NGX_CONF_UNSET;
        conf->size = NGX_CONF_UNSET_UINT;
        conf->limit = NGX_CONF_UNSET_UINT;
        return conf;
    }
    return NGX_CONF_ERROR;
}
static char *ngx_http_autoblacklist_merge_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_autoblacklist_loc_conf_t *prev = (ngx_http_autoblacklist_loc_conf_t *)parent;
    ngx_http_autoblacklist_loc_conf_t *conf = (ngx_http_autoblacklist_loc_conf_t *)child;
    ngx_conf_merge_value(conf->enable, prev->enable, (ngx_flag_t)0);
    ngx_conf_merge_sec_value(conf->expires, prev->expires, (ngx_int_t)86400);
    ngx_conf_merge_uint_value(conf->size, prev->size, (size_t)1024);
    ngx_conf_merge_uint_value(conf->limit, prev->limit, (size_t)30);
    if (!BLACKLIST)
    {
        BLACKLIST = std::make_shared<lru11::Cache<std::string, std::shared_ptr<black_ip_t>>>(conf->size);
    }
    return NGX_CONF_OK;
}
static ngx_int_t ngx_http_autoblacklist_handler(ngx_http_request_t *r)
{
    ngx_http_autoblacklist_loc_conf_t *conf = (ngx_http_autoblacklist_loc_conf_t *)ngx_http_get_module_loc_conf(r, ngx_http_autoblacklist_module);
    if (conf->enable == 0)
    {
        return NGX_DECLINED;
    }
    std::string ip = (char *)r->connection->addr_text.data;
    std::shared_ptr<black_ip_t> black_ip;
    if (BLACKLIST->tryGet(ip, black_ip))
    {
        double diff = difftime(time(0), black_ip->t);
        if (black_ip->disallow)
        {
            if (diff < conf->expires)
            {
                return NGX_HTTP_FORBIDDEN;
            }
            else
            {
                black_ip->disallow = false;
                black_ip->count = 1;
                black_ip->t = time(0);
            }
        }

        if ((diff == 0 && black_ip->count > conf->limit) || (diff > 0 && black_ip->count / diff > conf->limit))
        {
            black_ip->t = time(0);
            black_ip->disallow = true;
            return NGX_HTTP_FORBIDDEN;
        }
        else
        {
            black_ip->count++;
        }
    }
    else
    {
        BLACKLIST->insert(ip, std::make_shared<black_ip_t>());
    }
    return NGX_DECLINED;
}