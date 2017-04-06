#ifndef CACHE_HPP
#define CACHE_HPP

#include <string>
#include <Poco/UniqueExpireLRUCache.h>
#include <Poco/ExpirationDecorator.h>

namespace nginx {
    typedef Poco::ExpirationDecorator<std::string> cache_string;
    typedef Poco::UniqueExpireLRUCache<std::string, cache_string> cache;

}



#endif /* CACHE_HPP */

