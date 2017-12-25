#ifndef _LRUCACHE_HPP_INCLUDED_
#define _LRUCACHE_HPP_INCLUDED_

#include "LRUCache11.hpp"

namespace hi {
    namespace cache {

        template<typename key_t, typename value_t>
        class lru_cache {
        public:

            lru_cache(size_t max_size) : _cache_(max_size, 0) {

            }

            void put(const key_t& key, const value_t& value) {
                this-> _cache_.insert(key, value);
            }

            const value_t& get(const key_t& key) {
                return this-> _cache_.get(key);
            }

            void erase(const key_t& key) {
                this->_cache_.remove(key);
            }

            size_t size()const {
                return this->_cache_.size();
            }

            bool exists(const key_t& key) {
                return this->_cache_.contains(key);
            }

            virtual~lru_cache() {

            }
        private:
            lru11::Cache<key_t, value_t> _cache_;
        };

    } // namespace cache

}//namespace hi

#endif /* _LRUCACHE_HPP_INCLUDED_ */

