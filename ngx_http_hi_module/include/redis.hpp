#ifndef REDIS_HPP
#define REDIS_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <hiredis/hiredis.h>

namespace hi {

    class redis {
    public:

        redis() :
        content(0)
        , host()
        , port(0) {
        }

        virtual~redis() {
            if (this->content) {
                redisFree(this->content);
            }
        }

        void connect(const std::string& host = "127.0.0.1", int port = 6379) {
            this->host = host;
            this->port = port;
            this->content = redisConnect(host.c_str(), port);
            if (this->content && this->content->err == 0) {
                redisEnableKeepAlive(this->content);
            }
        }

        void reconnect() {
            if (!this->is_connected()) {
                redisFree(this->content);
                this->content = redisConnect(this->host.c_str(), this->port);
                if (this->content && this->content->err == 0) {
                    redisEnableKeepAlive(this->content);
                }
            }
        }

        bool is_connected()const {
            return this->content && this->content->err == 0;
        }

        std::string get(const std::string& key, bool &has) {
            std::string result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "GET %s", key.c_str());
            if (reply->type != REDIS_REPLY_NIL) {
                result.assign(reply->str, reply->len);
                has = true;
            } else {
                has = false;
            }
            freeReplyObject(reply);
            return result;
        }

        void set(const std::string& key, const std::string& value) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "SET %s %s", key.c_str(), value.c_str());
            freeReplyObject(reply);
        }

        void setex(const std::string& key, const std::string& value, long long expires) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "SETEX %s %lld %s", key.c_str(), expires, value.c_str());
            freeReplyObject(reply);
        }

        std::string getset(const std::string& key, const std::string& value, bool& has) {
            std::string result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "GETSET %s %s", key.c_str(), value.c_str());
            if (reply->type != REDIS_REPLY_NIL) {
                result.assign(reply->str, reply->len);
                has = true;
            } else {
                has = false;
            }
            freeReplyObject(reply);
            return result;
        }

        bool exists(const std::string& key) {
            bool result = false;
            redisReply* reply = (redisReply*) redisCommand(this->content, "EXISTS %s", key.c_str());
            result = reply->integer ? true : false;
            freeReplyObject(reply);
            return result;
        }

        void append(const std::string& key, const std::string& value) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "APPEND %s %s", key.c_str(), value.c_str());
            freeReplyObject(reply);
        }

        void del(const std::string& key) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "DEL %s", key.c_str());
            freeReplyObject(reply);
        }

        void expire(const std::string& key, long long expires) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "EXPIRE %s %lld", key.c_str(), expires);
            freeReplyObject(reply);
        }

        long long ttl(const std::string& key) {
            long long t = 0;
            redisReply* reply = (redisReply*) redisCommand(this->content, "TTL %s", key.c_str());
            t = reply->integer;
            freeReplyObject(reply);
            return t;
        }

        void incr(const std::string& key) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "INCR %s", key.c_str());
            freeReplyObject(reply);
        }

        void incrby(const std::string& key, long long v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "INCRBY %s %lld", key.c_str(), v);
            freeReplyObject(reply);
        }

        void incrbyfloat(const std::string& key, double v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "INCRBYFLOAT %s %e", key.c_str(), v);
            freeReplyObject(reply);
        }

        void decr(const std::string& key) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "DECR %s", key.c_str());
            freeReplyObject(reply);
        }

        void decrby(const std::string& key, long long v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "DECR %s %lld", key.c_str(), v);
            freeReplyObject(reply);
        }

        std::string hget(const std::string& key, const std::string& field, bool& has) {
            std::string result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "HGET %s %s", key.c_str(), field.c_str());
            if (reply->type != REDIS_REPLY_NIL) {
                result.assign(reply->str, reply->len);
                has = true;
            } else {
                has = false;
            }
            freeReplyObject(reply);
            return result;
        }

        void hset(const std::string& key, const std::string& field, const std::string& value) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
            freeReplyObject(reply);
        }

        void hdel(const std::string& key, const std::string& field) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HDEL %s %s", key.c_str(), field.c_str());
            freeReplyObject(reply);
        }

        bool hexists(const std::string& key, const std::string& field) {
            bool result = false;
            redisReply* reply = (redisReply*) redisCommand(this->content, "HEXISTS %s %s", key.c_str(), field.c_str());
            result = reply->integer ? true : false;
            freeReplyObject(reply);
            return result;
        }

        void hgetall(const std::string& key, std::unordered_map<std::string, std::string>& kvlist) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HGETALL %s ", key.c_str());
            std::string k, v;
            for (size_t i = 0; i < reply->elements; ++++i) {
                kvlist[k.assign(reply->element[i]->str, reply->element[i]->len)] = v.assign(reply->element[i + 1]->str, reply->element[i + 1]->len);
            }
            freeReplyObject(reply);
        }

        void hkeys(const std::string& key, std::vector<std::string>& klist) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HKEYS %s ", key.c_str());
            std::string k;
            for (size_t i = 0; i < reply->elements; ++i) {
                klist.push_back(k.assign(reply->element[i]->str, reply->element[i]->len));
            }
            freeReplyObject(reply);
        }

        void hvals(const std::string& key, std::vector<std::string>& vlist) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HVALS %s ", key.c_str());
            std::string v;
            for (size_t i = 0; i < reply->elements; ++i) {
                vlist.push_back(v.assign(reply->element[i]->str, reply->element[i]->len));
            }
            freeReplyObject(reply);
        }

        void hincrby(const std::string& key, const std::string& field, long long v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HINCRBY %s %s %lld", key.c_str(), field.c_str(), v);
            freeReplyObject(reply);
        }

        void hincrbyfloat(const std::string& key, const std::string& field, double v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "HINCRBYFLOAT %s %s %e", key.c_str(), field.c_str(), v);
            freeReplyObject(reply);
        }

        long long hlen(const std::string& key) {
            long long result = 0;
            redisReply* reply = (redisReply*) redisCommand(this->content, "HLEN %s", key.c_str());
            result = reply->integer;
            freeReplyObject(reply);
            return result;
        }

        void hmset(const std::string& key, const std::unordered_map<std::string, std::string>& kvlist) {
            std::string cmd("HMSET " + key + " ");
            for (const auto& item : kvlist) {
                cmd.append(item.first + " " + item.second + " ");
            }
            redisReply* reply = (redisReply*) redisCommand(this->content, cmd.c_str());
            freeReplyObject(reply);
        }

        void hmget(const std::string& key, std::vector<std::string>& flist) {
            std::string cmd("HMGET " + key + " ");
            for (const auto& item : flist) {
                cmd.append(item + " ");
            }
            redisReply* reply = (redisReply*) redisCommand(this->content, cmd.c_str());
            std::string v;
            for (size_t i = 0; i < reply->elements; ++i) {
                flist[i] = v.assign(reply->element[i]->str, reply->element[i]->len);
            }
            freeReplyObject(reply);
        }

        void lpush(const std::string& key, const std::vector<std::string>& vlist) {
            std::string cmd("LPUSH " + key + " ");
            for (const auto& item : vlist) {
                cmd.append(item + " ");
            }
            redisReply* reply = (redisReply*) redisCommand(this->content, cmd.c_str());
            freeReplyObject(reply);
        }

        std::string lpop(const std::string& key) {
            std::string result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "LPOP %s", key.c_str());
            result.assign(reply->str, reply->len);
            freeReplyObject(reply);
            return result;
        }

        long long llen(const std::string& key) {
            long long result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "LLEN %s", key.c_str());
            result = reply->integer;
            freeReplyObject(reply);
            return result;
        }

        void lrange(const std::string& key, long long start, long long end, std::vector<std::string>& list) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "LRANGE %s %d %d", key.c_str(), start, end);
            std::string v;
            for (size_t i = 0; i < reply->elements; ++i) {
                v.assign(reply->element[i]->str, reply->element[i]->len);
                list.push_back(v);
            }
            freeReplyObject(reply);
        }

        void lrem(const std::string& key, long long c, const std::string& v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "LREM %s %d %s", key.c_str(), c, v.c_str());
            freeReplyObject(reply);
        }

        void lset(const std::string& key, long long i, const std::string& v) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "LSET %s %d %s", key.c_str(), i, v.c_str());
            freeReplyObject(reply);
        }

        void rpush(const std::string& key, const std::vector<std::string>& vlist) {
            std::string cmd("RPUSH " + key + " ");
            for (const auto& item : vlist) {
                cmd.append(item + " ");
            }
            redisReply* reply = (redisReply*) redisCommand(this->content, cmd.c_str());
            freeReplyObject(reply);
        }

        std::string rpop(const std::string& key) {
            std::string result;
            redisReply* reply = (redisReply*) redisCommand(this->content, "RPOP %s", key.c_str());
            result.assign(reply->str, reply->len);
            freeReplyObject(reply);
            return result;
        }

        void rename(const std::string& old_key, const std::string& new_key) {
            redisReply* reply = (redisReply*) redisCommand(this->content, "RENAME %s %s", old_key.c_str(), new_key.c_str());
            freeReplyObject(reply);
        }
    private:
        redisContext* content;
        std::string host;
        int port;
    };
}




#endif /* REDIS_HPP */

