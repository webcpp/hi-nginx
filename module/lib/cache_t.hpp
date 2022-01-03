#pragma once

#include <string>
#include <ctime>
#include <memory>
#include "LRUCache11.hpp"

namespace hi
{

    class cache_t
    {
    private:
        static std::shared_ptr<lru11::Cache<std::string, cache_t>> cache_manager;

    public:
        cache_t() : status(200), t(time(0)), content_type(), content()
        {
        }
        ~cache_t() = default;
        int status;
        time_t t;
        std::string content_type, content;

        bool expired(long long expires) const;

        static std::shared_ptr<lru11::Cache<std::string, cache_t>> get_cache_manager(size_t len = 1024);
    };
} // namespace hi
