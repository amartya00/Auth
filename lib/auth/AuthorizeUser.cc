#include "Auth.h"
#include "DataStructures.h"
#include "Exceptions.h"

bool Sigsegv::Auth::Service::authorizeUser(const std::string& uid, const std::string& pwd, const std::string& serviceName) const {
    Sigsegv::Auth::UserItem item;
    item.setUid(uid);
    item.setPwd(pwd);
    if (isWhitelistingEnabledForService(serviceName)) {
        if (!isUserWhitelisted(item, serviceName)){
            return false;
        }
    }

    auto returnedUserData = isUserRegisteredForService(item, serviceName);
    for (auto s : returnedUserData.getAuthorizedServices()) {
        if (s.compare(serviceName) == 0) {
            // If already registered
            return true;
        }
    }
    return false;
}
