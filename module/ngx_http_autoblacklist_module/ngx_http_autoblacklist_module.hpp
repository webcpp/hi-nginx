#pragma once

extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include "LRUCache11.hpp"
#include <memory>
#include <string>

class black_ip_t
{
public:
    black_ip_t()
        : t(time(0)), count(1), disallow(false)
    {
    }
    virtual ~black_ip_t() = default;
    time_t t;
    size_t count;
    bool disallow;
};

static std::shared_ptr<lru11::Cache<std::string, std::shared_ptr<black_ip_t>>> BLACKLIST;

typedef struct
{
    size_t size, limit;
    ngx_flag_t enable;
    ngx_int_t expires;
} ngx_http_autoblacklist_loc_conf_t;
