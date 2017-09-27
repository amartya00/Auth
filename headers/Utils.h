#ifndef SIGSEGV_UTILS
#define SIGSEGV_UTILS

#include <aws/core/Aws.h>
#include <string>

namespace Sigsegv {
    namespace Utils {
        Aws::String toAwsString (const std::string&);
        std::string toStdString (const Aws::String&);
    }
}

#endif
