#ifndef SESSION_HPP
#define SESSION_HPP


#include <string>
#include <map>
#include <Poco/UniqueExpireLRUCache.h>
#include <Poco/ExpirationDecorator.h>
#include <Poco/DynamicAny.h>

#define SESSION_ID_NAME "NGINXCPPSESSION"

namespace nginx {

    typedef Poco::ExpirationDecorator<std::map<std::string, Poco::DynamicAny>> session_map;
    typedef Poco::UniqueExpireLRUCache<std::string, session_map> session;

}


#endif /* SESSION_HPP */

