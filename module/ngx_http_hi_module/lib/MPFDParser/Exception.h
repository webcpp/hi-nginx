#ifndef _EXCEPTION_H
#define	_EXCEPTION_H

#include <string>
#include <iostream>
#include <errno.h>


namespace MPFD {

    class Exception {
    public:
        Exception(const std::string& error);
        Exception(const Exception& orig);
        virtual ~Exception()=default;
        
        const std::string& GetError()const;
     
    private:
        std::string Error;

    };
}

#endif	/* _EXCEPTION_H */

