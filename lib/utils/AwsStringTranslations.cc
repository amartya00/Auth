#include "Utils.h"

#include <aws/core/Aws.h>
#include <string>

Aws::String Sigsegv::Utils::toAwsString(const std::string& input) {
    Aws::String retval(input.c_str());
    return retval;
}

std::string Sigsegv::Utils::toStdString(const Aws::String& input) {
    std::string retval(input.c_str());
    return retval;
}
