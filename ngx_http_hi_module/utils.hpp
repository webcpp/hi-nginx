#ifndef UTILS_HPP
#define UTILS_HPP

extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <openssl/md5.h>

#include <memory>
#include <string>
#include <ctime>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include "include/request.hpp"
#include "lib/MPFDParser/Parser.h"
#include "lib/msgpack/msgpack.hpp"


namespace hi {

    static std::string md5(const std::string& str) {
        unsigned char digest[16] = {0};
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, str.c_str(), str.size());
        MD5_Final(digest, &ctx);

        unsigned char tmp[32] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = 16;
        while (len--) {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char*) tmp, 32);
    }

    static std::string random_string(const std::string& s) {
        time_t now = time(NULL);
        char* now_str = ctime(&now);

        return md5(s + now_str);
    }

    static bool is_dir(const std::string& s) {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }

    static bool is_file(const std::string& s) {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISREG(st.st_mode);
    }

    static std::string read_file(const std::string& path) {
        std::ifstream ifs(path.c_str());
        return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    }

    static bool upload(hi::request& req, ngx_str_t* body, ngx_http_core_loc_conf_t* clcf, ngx_http_request_t *r, const std::string& temp_dir, std::string & err_msg) {
        bool result = false;
        try {
            if ((is_dir(temp_dir) || mkdir(temp_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)) {
                std::shared_ptr<MPFD::Parser> POSTParser(new MPFD::Parser());
                POSTParser->SetTempDirForFileUpload(temp_dir);
                POSTParser->SetUploadedFilesStorage(MPFD::Parser::StoreUploadedFilesInFilesystem);
                POSTParser->SetMaxCollectedDataLength(clcf->client_max_body_size);
                POSTParser->SetContentType((char*) r->headers_in.content_type->value.data);
                POSTParser->AcceptSomeData((char*) body->data, body->len);
                auto fields = POSTParser->GetFieldsMap();

                for (auto &item : fields) {
                    if (item.second->GetType() == MPFD::Field::TextType) {
                        req.form.insert(std::make_pair(item.first, item.second->GetTextTypeContent()));
                    } else {
                        std::string upload_file_name = item.second->GetFileName(), ext;
                        std::string::size_type p = upload_file_name.find_last_of(".");
                        if (p != std::string::npos) {
                            ext = std::move(upload_file_name.substr(p));
                        }
                        std::string temp_file = std::move(temp_dir + ("/" + hi::random_string(req.client + item.second->GetFileName()).append(ext)));
                        rename(item.second->GetTempFileName().c_str(), temp_file.c_str());
                        req.form.insert(std::make_pair(item.first, temp_file));
                    }
                }
                result = true;
            }
        } catch (MPFD::Exception& err) {
            err_msg = err.GetError();
        }
        return result;
    }

    static std::string serialize(const std::unordered_map<std::string, std::string>& m) {
        std::stringstream ss;
        msgpack::pack(ss, m);
        ss.seekg(0);
        return ss.str();
    }

    static void deserialize(const std::string& str, std::unordered_map<std::string, std::string>& m) {
        msgpack::unpack(str.c_str(), str.size()).get().convert(m);
    }
}

#endif /* UTILS_HPP */

