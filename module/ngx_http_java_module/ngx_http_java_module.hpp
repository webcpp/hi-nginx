#pragma once

extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <regex>
#include <memory>

#include "java.hpp"

static std::shared_ptr<std::regex> java_uri_re = nullptr;
static std::shared_ptr<hi::java> java_engine = nullptr;

typedef struct
{
    ngx_str_t class_path;
    ngx_str_t options;
    ngx_str_t servlet;
    ngx_str_t uri_pattern;
    ngx_int_t expires;
    ngx_int_t version;
} ngx_http_java_loc_conf_t;