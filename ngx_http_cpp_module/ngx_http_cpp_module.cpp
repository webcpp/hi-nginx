extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include "include/request.hpp"
#include "include/response.hpp"
#include "lib/plugin.hpp"
#include "lib/route.hpp"
#include "lib/upload_handler.hpp"
#include "lib/cpp_tools.hpp"


#ifndef SESSION_ID_NAME
#define SESSION_ID_NAME "SESSION"
#endif

static std::vector<hi::cpp_tools_t> CPP_TOOLS;

typedef struct {
    ngx_str_t route_file;
    ngx_str_t module_dir;
    ngx_str_t upload_dir;
    ngx_str_t upload_field;
    ngx_str_t upload_type;
    size_t upload_size;
    ngx_uint_t session_expire;
    ngx_int_t session_size;
    ngx_int_t cache_size;
    ngx_int_t cpp_tools_index;
} ngx_http_cpp_loc_conf_t;



static void * ngx_http_cpp_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_cpp_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child);
static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r);
static char *ngx_http_cpp_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static void get_input_headers(ngx_http_request_t* r, std::map<std::string, std::string>& input_headers);
static void set_output_headers(ngx_http_request_t* r, std::multimap<std::string, std::string>& output_headers);


static void get_input_cookies(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& cookies);
static void set_output_cookies(const std::map<std::string, std::string>& cookies, std::multimap<std::string, std::string>& output_headers, bool ssl);


static ngx_int_t normal_body_handler(ngx_http_request_t* r);
static void default_body_handler(ngx_http_request_t* r);
static void set_form(ngx_http_request_t* r, const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& form);

ngx_command_t ngx_http_cpp_commands[] = {
    {
        ngx_string("cpp_route_file"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, route_file),
        NULL
    },
    {
        ngx_string("cpp_module_dir"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, module_dir),
        NULL
    },
    {
        ngx_string("cpp_upload_dir"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, upload_dir),
        NULL
    },
    {
        ngx_string("cpp_upload_field"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, upload_field),
        NULL
    },
    {
        ngx_string("cpp_upload_type"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_str_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, upload_type),
        NULL
    },
    {
        ngx_string("cpp_upload_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_size_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, upload_size),
        NULL
    },
    {
        ngx_string("cpp_session_expire"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_msec_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, session_expire),
        NULL
    },
    {
        ngx_string("cpp_session_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, session_size),
        NULL
    },
    {
        ngx_string("cpp_cache_size"),
        NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_cpp_loc_conf_t, cache_size),
        NULL
    },
    {
        ngx_string("cpp"),
        NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
        ngx_http_cpp_conf_handler,
        NGX_HTTP_LOC_CONF_OFFSET,
        0,
        NULL
    },

    ngx_null_command
};


ngx_http_module_t ngx_http_cpp_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    ngx_http_cpp_create_loc_conf, /* create location configuration */
    ngx_http_cpp_merge_loc_conf /* merge location configuration */
};




ngx_module_t ngx_http_cpp_module = {
    NGX_MODULE_V1,
    &ngx_http_cpp_module_ctx, /* module context */
    ngx_http_cpp_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

static void * ngx_http_cpp_create_loc_conf(ngx_conf_t *cf) {
    ngx_http_cpp_loc_conf_t *conf = (ngx_http_cpp_loc_conf_t*) ngx_pcalloc(cf->pool, sizeof (ngx_http_cpp_loc_conf_t));
    if (conf) {
        conf->module_dir.len = 0;
        conf->module_dir.data = NULL;
        conf->route_file.len = 0;
        conf->route_file.data = NULL;
        conf->upload_dir.len = 0;
        conf->upload_dir.data = NULL;
        conf->upload_field.len = 0;
        conf->upload_field.data = NULL;
        conf->upload_type.len = 0;
        conf->upload_type.data = NULL;
        conf->upload_size = NGX_CONF_UNSET_SIZE;
        conf->session_expire = NGX_CONF_UNSET_MSEC;
        conf->session_size = NGX_CONF_UNSET;
        conf->cache_size = NGX_CONF_UNSET;

        hi::cpp_tools_t cpp_tools;
        CPP_TOOLS.push_back(cpp_tools);
        conf->cpp_tools_index = CPP_TOOLS.size() - 1;

        return conf;
    }
    return NGX_CONF_ERROR;
}

static char * ngx_http_cpp_merge_loc_conf(ngx_conf_t* cf, void* parent, void* child) {
    ngx_http_cpp_loc_conf_t * prev = (ngx_http_cpp_loc_conf_t*) parent;
    ngx_http_cpp_loc_conf_t * conf = (ngx_http_cpp_loc_conf_t*) child;

    ngx_conf_merge_str_value(conf->module_dir, prev->module_dir, "");
    ngx_conf_merge_str_value(conf->route_file, prev->route_file, "");
    ngx_conf_merge_str_value(conf->upload_dir, prev->upload_dir, "");
    ngx_conf_merge_str_value(conf->upload_field, prev->upload_field, "upload");
    ngx_conf_merge_str_value(conf->upload_type, prev->upload_type, "image/png|image/jpeg|image/gif|application/pdf|application/zip");
    ngx_conf_merge_size_value(conf->upload_size, prev->upload_size, (size_t) 10 * 1024);
    ngx_conf_merge_msec_value(conf->session_expire, prev->session_expire, (ngx_uint_t) 60 * 60 * 1000);
    ngx_conf_merge_value(conf->session_size, prev->session_size, (ngx_int_t) 1024);
    ngx_conf_merge_value(conf->cache_size, prev->cache_size, (ngx_int_t) 1024);

    hi::cpp_tools_t &cpp_tools = CPP_TOOLS[conf->cpp_tools_index];
    cpp_tools.CLASS_LOADER.assign(new Poco::ClassLoader<hi::view>);
    cpp_tools.PLUGIN.assign(new hi::plugin(std::string((char*) conf->module_dir.data, conf->module_dir.len), cpp_tools.CLASS_LOADER.get()));
    cpp_tools.ROUTER.assign(new hi::route(std::string((char*) conf->route_file.data, conf->route_file.len)));
    cpp_tools.CACHE.assign(new hi::cache(conf->cache_size));
    cpp_tools.SESSION.assign(new hi::session(conf->session_size));

    return NGX_CONF_OK;
}

static ngx_int_t ngx_http_cpp_handler(ngx_http_request_t *r) {
    if (r->headers_in.content_length_n) {
        r->request_body_in_file_only = 1;
        r->request_body_in_persistent_file = 1;
        return ngx_http_read_client_request_body(r, default_body_handler);
    } else {
        ngx_http_discard_request_body(r);
        return normal_body_handler(r);
    }
}

static char *ngx_http_cpp_conf_handler(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;
    clcf = (ngx_http_core_loc_conf_t *) ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_cpp_handler;
    return NGX_CONF_OK;
}

static void get_input_headers(ngx_http_request_t* r, std::map<std::string, std::string>& input_headers) {
    ngx_table_elt_t *th;
    ngx_list_part_t *part;
    part = &r->headers_in.headers.part;
    th = (ngx_table_elt_t*) part->elts;
    ngx_uint_t i;
    for (i = 0; /* void */; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            th = (ngx_table_elt_t*) part->elts;
            i = 0;
        }
        input_headers[(char*) th[i].key.data] = (char*) th[i].value.data;
    }
    if (input_headers.find("Connection") != input_headers.end()) {
        r->keepalive = 1;
    }
    ngx_http_cpp_loc_conf_t *conf;
    conf = (ngx_http_cpp_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_cpp_module);
    input_headers["cpp_module_dir"] = (char*) conf->module_dir.data;
    input_headers["cpp_route_file"] = (char*) conf->route_file.data;
    input_headers["cpp_upload_dir"] = (char*) conf->upload_dir.data;
    input_headers["cpp_upload_field"] = (char*) conf->upload_field.data;
    input_headers["cpp_upload_type"] = (char*) conf->upload_type.data;
    input_headers["cpp_upload_size"] = Poco::NumberFormatter::format(conf->upload_size);
    input_headers["cpp_session_expire"] = Poco::NumberFormatter::format(conf->session_expire);

    input_headers["path"].assign((char*) r->uri.data, r->uri.len);
    input_headers["query"].assign((char*) r->args.data, r->args.len);
    input_headers["uri"].assign((char*) r->unparsed_uri.data, r->unparsed_uri.len);
    input_headers["method"].assign((char*) r->method_name.data, r->method_name.len);
    input_headers["client"].assign((char*) r->connection->addr_text.data, r->connection->addr_text.len);
}

static void set_output_headers(ngx_http_request_t* r, std::multimap<std::string, std::string>& output_headers) {
    for (auto& item : output_headers) {
        ngx_table_elt_t * h = (ngx_table_elt_t *) ngx_list_push(&r->headers_out.headers);
        if (h) {
            h->hash = 1;
            h->key.data = (u_char*) item.first.c_str();
            h->key.len = item.first.size();
            h->value.data = (u_char*) item.second.c_str();
            h->value.len = item.second.size();
        }
    }

}

static void get_input_cookies(const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& cookies) {
    if (input_headers.find("Cookie") != input_headers.end()) {
        const std::string& cookie_header = input_headers.at("Cookie");
        std::string cookie_string = "cookie;" + cookie_header, tmp;
        Poco::Net::NameValueCollection nvc;
        Poco::Net::HTTPMessage::splitParameters(cookie_string, tmp, nvc);
        for (auto & item : nvc) {
            cookies[item.first] = item.second;
        }
    }
}

static void set_output_cookies(const std::map<std::string, std::string>& cookies, std::multimap<std::string, std::string>& output_headers, bool ssl) {
    std::string cookie_header("Set-Cookie");
    for (auto & item : cookies) {
        Poco::Net::HTTPCookie cookie(item.first, item.second);
        cookie.setMaxAge(-1);
        cookie.setPath("/");
        cookie.setHttpOnly(true);
        cookie.setSecure(ssl);
        //cookie.setVersion(1);

        output_headers.insert(std::make_pair(cookie_header, cookie.toString()));
    }


}

static ngx_int_t normal_body_handler(ngx_http_request_t * r) {
    hi::request ngx_request;
    hi::response ngx_response;
    get_input_headers(r, ngx_request.headers);
    get_input_cookies(ngx_request.headers, ngx_request.cookies);
    set_form(r, ngx_request.headers, ngx_request.form);

    ngx_http_cpp_loc_conf_t * conf = (ngx_http_cpp_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_cpp_module);
    hi::cpp_tools_t &cpp_tools = CPP_TOOLS[conf->cpp_tools_index];
    std::string class_name = cpp_tools.ROUTER->get_route(ngx_request.headers["method"], ngx_request.headers["uri"], ngx_request.route);
    Poco::SharedPtr<hi::view> view_instance;
    if (!class_name.empty()) {
        //        if (cpp_tools.HANDLER.find(class_name) == cpp_tools.HANDLER.end()) {
        auto finded = cpp_tools.CLASS_LOADER->findClass(class_name);
        if (finded && finded->canCreate()) {
            view_instance = cpp_tools.CLASS_LOADER->create(class_name);
            //                cpp_tools.HANDLER[class_name] = view_instance;
        }
        //        } else {
        //            view_instance = cpp_tools.HANDLER[class_name];
        //        }
    }


    if (view_instance.isNull()) {
        ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, ("Failed to allocate view_instance " + class_name).c_str());

        return NGX_HTTP_NOT_FOUND;
    }

    std::string session_id;
    if (ngx_request.cookies.find(SESSION_ID_NAME) == ngx_request.cookies.end()) {
        session_id = Poco::UUIDGenerator::defaultGenerator().createRandom().toString();
        ngx_response.cookies[SESSION_ID_NAME] = session_id;
    } else {
        session_id = ngx_request.cookies[SESSION_ID_NAME];
    }


    if (!cpp_tools.SESSION->has(session_id)) {
        cpp_tools.SESSION->add(session_id, hi::session_map(std::map<std::string, Poco::DynamicAny>(), conf->session_expire));
    }

    ngx_response.cache = cpp_tools.CACHE.get();
    ngx_response.session = &(cpp_tools.SESSION->get(session_id)->value());

    view_instance->handler(ngx_request, ngx_response);

    set_output_cookies(ngx_response.cookies, ngx_response.headers, (r->http_connection->ssl ? true : false));


    ngx_str_t response;
    response.data = (u_char*) ngx_response.content.c_str();
    response.len = ngx_response.content.size();


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

    set_output_headers(r, ngx_response.headers);
    r->headers_out.status = ngx_response.status;
    r->headers_out.content_length_n = response.len;

    ngx_int_t rc;
    rc = ngx_http_send_header(r);
    if (rc != NGX_OK) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    return ngx_http_output_filter(r, &out);
}

static void default_body_handler(ngx_http_request_t * r) {
    ngx_http_finalize_request(r, normal_body_handler(r));
}

static void set_form(ngx_http_request_t* r, const std::map<std::string, std::string>& input_headers, std::map<std::string, std::string>& form) {
    Poco::Net::HTTPRequest poco_req;
    poco_req.setMethod(input_headers.at("method"));
    poco_req.setURI(input_headers.at("uri"));
    poco_req.setVersion(Poco::Net::HTTPRequest::HTTP_1_1);
    std::string content_type;
    if (input_headers.find("Content-Type") != input_headers.end()) {
        content_type = input_headers.at("Content-Type");
        poco_req.setContentType(content_type);
    }
    Poco::Net::HTMLForm* poco_form = 0;
    if (r->method & (NGX_HTTP_GET)) {
        poco_form = new Poco::Net::HTMLForm(poco_req);
    } else if (r->method & (NGX_HTTP_POST | NGX_HTTP_PUT)) {
        std::string body_temp_file_path = (char*) r->request_body->temp_file->file.name.data;
        Poco::File temp_file(body_temp_file_path);
        if (temp_file.exists() && temp_file.canRead()) {
            Poco::FileInputStream is(body_temp_file_path);
            if (content_type.find("multipart/form-data") == std::string::npos) {
                poco_form = new Poco::Net::HTMLForm(poco_req, is);
            } else {
                ngx_http_cpp_loc_conf_t *conf;
                conf = (ngx_http_cpp_loc_conf_t *) ngx_http_get_module_loc_conf(r, ngx_http_cpp_module);
                std::string allow_name = (char*) conf->upload_field.data
                        , allow_type = (char*) conf->upload_type.data
                        , upload_dir = (char*) conf->upload_dir.data;
                double upload_size = (double) conf->upload_size;
                hi::upload_handler upload_handler(allow_name, allow_type, upload_dir, upload_size);
                poco_form = new Poco::Net::HTMLForm(poco_req, is, upload_handler);
                auto result = upload_handler.get_data();
                for (auto & item : result) {
                    if (item.ok) {
                        form[item.name] = item.webpath;
                    }
                }
            }

            temp_file.remove();

        }
    }
    if (poco_form) {
        for (auto & item : *poco_form) {
            form[item.first] = item.second;
        }
        delete poco_form;
    }

}
