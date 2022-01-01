#include "Exception.h"

MPFD::Exception::Exception(const std::string& error) {
    Error = error;
}

MPFD::Exception::Exception(const MPFD::Exception& orig) {
    if (&orig != this) {
        Error = orig.Error;
    }
}

const std::string& MPFD::Exception::GetError() const {
    return Error;
}