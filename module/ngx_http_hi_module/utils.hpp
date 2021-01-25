#pragma once

extern "C"
{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <ctime>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/request.hpp"
#include "lib/MPFDParser/Parser.h"
#include "lib/msgpack/msgpack.hpp"

namespace hi
{

    static std::string md5(const std::string &str)
    {
        unsigned char digest[MD5_DIGEST_LENGTH] = {0};
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, str.c_str(), str.size());
        MD5_Final(digest, &ctx);

        unsigned char tmp[MD5_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = MD5_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, MD5_DIGEST_LENGTH * 2);
    }

    static std::string sha1(const std::string &str)
    {
        unsigned char digest[SHA_DIGEST_LENGTH] = {0};
        SHA_CTX ctx;
        SHA1_Init(&ctx);
        SHA1_Update(&ctx, str.c_str(), str.size());
        SHA1_Final(digest, &ctx);

        unsigned char tmp[SHA_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = SHA_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, SHA_DIGEST_LENGTH * 2);
    }

    static std::string sha256(const std::string &str)
    {
        unsigned char digest[SHA256_DIGEST_LENGTH] = {0};
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, str.c_str(), str.size());
        SHA256_Final(digest, &ctx);

        unsigned char tmp[SHA256_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = SHA256_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, SHA256_DIGEST_LENGTH * 2);
    }

    static std::string sha512(const std::string &str)
    {
        unsigned char digest[SHA512_DIGEST_LENGTH] = {0};
        SHA512_CTX ctx;
        SHA512_Init(&ctx);
        SHA512_Update(&ctx, str.c_str(), str.size());
        SHA512_Final(digest, &ctx);

        unsigned char tmp[SHA512_DIGEST_LENGTH * 2] = {0}, *dst = &tmp[0], *src = &digest[0];
        unsigned char hex[] = "0123456789abcdef";
        int len = SHA512_DIGEST_LENGTH;
        while (len--)
        {

            *dst++ = hex[*src >> 4];
            *dst++ = hex[*src++ & 0xf];
        }

        return std::string((char *)tmp, SHA512_DIGEST_LENGTH * 2);
    }

    class aes
    {
    public:
        aes(const std::string &key)
            : key(key), en_key(), de_key()
        {
            AES_set_encrypt_key((const unsigned char *)this->key.c_str(), 128, &this->en_key);
            AES_set_decrypt_key((const unsigned char *)this->key.c_str(), 128, &this->de_key);
        }
        virtual ~aes() = default;

    private:
        std::string encode16(const std::string &str)
        {
            unsigned char out[AES_BLOCK_SIZE];
            AES_encrypt((const unsigned char *)str.c_str(), out, &this->en_key);
            return std::string((char *)out, AES_BLOCK_SIZE);
        }
        std::string decode16(const std::string &cipher)
        {
            unsigned char out[AES_BLOCK_SIZE];
            AES_decrypt((const unsigned char *)cipher.c_str(), out, &this->de_key);
            return std::string((char *)out, AES_BLOCK_SIZE);
        }

        void trim(std::string &s)
        {
            if (!s.empty())
            {
                s.erase(0, s.find_first_not_of(" "));
                s.erase(s.find_last_not_of(" ") + 1);
            }
        }

    public:
        const std::string &encode(const std::string &str)
        {
            this->cipher.clear();
            size_t len = str.size(), count = len / AES_BLOCK_SIZE, j = 0;
            if (count > 0)
            {
                for (size_t i = 0; i < count; ++i)
                {
                    this->cipher.append(this->encode16(str.substr(j, AES_BLOCK_SIZE)));
                    j += AES_BLOCK_SIZE;
                }
                this->cipher.append(this->encode16(str.substr(j)));
            }
            else
            {
                this->cipher = this->encode16(str);
            }
            return this->cipher;
        }
        const std::string &decode(const std::string &str)
        {
            this->plain.clear();
            size_t len = str.size(), count = len / AES_BLOCK_SIZE, j = 0;
            if (count > 0)
            {
                for (size_t i = 0; i < count; ++i)
                {
                    this->plain.append(this->decode16(str.substr(j, AES_BLOCK_SIZE)));
                    j += AES_BLOCK_SIZE;
                }
            }
            else
            {
                this->plain = this->decode16(str);
            }
            this->trim(this->plain);
            return this->plain;
        }

    private:
        std::string key, cipher, plain;
        AES_KEY en_key, de_key;
    };

    static std::string random_string(const std::string &s)
    {
        time_t now = time(NULL);
        char *now_str = ctime(&now);

        return md5(s + now_str);
    }

    static bool is_dir(const std::string &s)
    {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }

    static bool is_file(const std::string &s)
    {
        struct stat st;

        return stat(s.c_str(), &st) == 0 && S_ISREG(st.st_mode);
    }

    static std::string read_file(const std::string &path)
    {
        std::ifstream ifs(path.c_str());
        return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    }

    static bool upload(hi::request &req, ngx_str_t *body, ngx_http_core_loc_conf_t *clcf, ngx_http_request_t *r, const std::string &temp_dir, std::string &err_msg)
    {
        bool result = false;
        try
        {
            if ((is_dir(temp_dir) || mkdir(temp_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0))
            {
                std::shared_ptr<MPFD::Parser> POSTParser(new MPFD::Parser());
                POSTParser->SetTempDirForFileUpload(temp_dir);
                POSTParser->SetUploadedFilesStorage(MPFD::Parser::StoreUploadedFilesInFilesystem);
                POSTParser->SetMaxCollectedDataLength(clcf->client_max_body_size);
                POSTParser->SetContentType((char *)r->headers_in.content_type->value.data);
                POSTParser->AcceptSomeData((char *)body->data, body->len);
                auto fields = POSTParser->GetFieldsMap();

                for (auto &item : fields)
                {
                    if (item.second->GetType() == MPFD::Field::TextType)
                    {
                        req.form.insert(std::make_pair(item.first, item.second->GetTextTypeContent()));
                    }
                    else
                    {
                        std::string upload_file_name = item.second->GetFileName(), ext;
                        std::string::size_type p = upload_file_name.find_last_of(".");
                        if (p != std::string::npos)
                        {
                            ext = std::move(upload_file_name.substr(p));
                        }
                        std::string temp_file = std::move(temp_dir + ("/" + hi::random_string(req.client + item.second->GetFileName()).append(ext)));
                        rename(item.second->GetTempFileName().c_str(), temp_file.c_str());
                        req.form.insert(std::make_pair(item.first, temp_file));
                    }
                }
                result = true;
            }
        }
        catch (MPFD::Exception &err)
        {
            err_msg = err.GetError();
        }
        return result;
    }

    static std::string serialize(const std::unordered_map<std::string, std::string> &m)
    {
        std::stringstream ss;
        msgpack::pack(ss, m);
        ss.seekg(0);
        return ss.str();
    }

    static void deserialize(const std::string &str, std::unordered_map<std::string, std::string> &m)
    {
        msgpack::unpack(str.c_str(), str.size()).get().convert(m);
    }
} // namespace hi
