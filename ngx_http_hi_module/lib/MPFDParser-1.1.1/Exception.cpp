// This file is distributed under GPLv3 licence
// Author: Gorelov Grigory (gorelov@grigory.info)
//
// Contacts and other info are on the WEB page:  grigory.info/MPFDParser


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