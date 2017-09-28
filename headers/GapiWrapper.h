#ifndef SIGSEGV_AUTH_GAPI_WRAPPER
#define SIGSEGV_AUTH_GAPI_WRAPPER

#include "DataStructures.h"

namespace Sigsegv {
    namespace Auth {
        class GapiWrapper {
        public:
            static size_t writeCallback (void* contents, size_t size, size_t nmemb, void* userp);
            virtual Sigsegv::Auth::UserItem getInfoFromOAuth (const std::string& accessToken) const;
        };
    }
}

#endif
