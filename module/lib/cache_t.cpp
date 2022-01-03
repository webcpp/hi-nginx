

#include "cache_t.hpp"

namespace hi
{

    std::shared_ptr<lru11::Cache<std::string, cache_t>> cache_t::cache_manager = nullptr;

    std::shared_ptr<lru11::Cache<std::string, cache_t>> cache_t::get_cache_manager(size_t len)
    {
        if (cache_t::cache_manager == nullptr)
        {
            cache_t::cache_manager = std::make_shared<lru11::Cache<std::string, cache_t>>(len);
        }
        return cache_t::cache_manager;
    }

    bool cache_t::expired(long long expires) const
    {
        return difftime(time(0), this->t) > expires;
    }

} // namespace hi