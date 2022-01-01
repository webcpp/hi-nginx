#pragma once 




extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <regex>
#include <memory>

#include "cpp.hpp"

static std::shared_ptr<std::regex> cpp_uri_re = nullptr;
static std::shared_ptr<hi::cpp> cpp_engine = nullptr;

typedef struct
{
    ngx_str_t search_path;
    ngx_str_t uri_pattern;
    ngx_int_t expires;
} ngx_http_cpp_loc_conf_t;