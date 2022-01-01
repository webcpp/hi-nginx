#pragma once

extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <regex>
#include <memory>

#include "lua.hpp"

static std::shared_ptr<std::regex> lua_uri_re = nullptr;
static std::shared_ptr<hi::lua> lua_engine = nullptr;

typedef struct
{
    ngx_str_t package_search_path;
    ngx_str_t cpackage_search_path;
    ngx_str_t uri_pattern;
    ngx_int_t expires;
    ngx_int_t memory_limit;
} ngx_http_lua_loc_conf_t;