#include "Auth.h"
#include "DataStructures.h"
#include "Utils.h"
#include "Exceptions.h"

#include <ctime>
#include <functional>

Sigsegv::Auth::UserItem Sigsegv::Auth::Service::signinOrRegister(const std::string &oauthToken, const std::string &serviceName) const {
    auto userData = gapiWrapper->getInfoFromOAuth(oauthToken);
    Sigsegv::Auth::Exceptions::ClientException cexc;

    if (isWhitelistingEnabledForService(serviceName)) {
        if (!isUserWhitelisted(userData, serviceName)) {
            cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::WHITELIST_ERROR);
            cexc.setMessage("User not whitelisted in a whitelisting enabled service");
            throw cexc;
        }
    }

    auto returnedUserData = IsUserRegisteredForService(userData, serviceName);
    for (auto s : returnedUserData.getAuthorizedServices()) {
        if (s.compare(serviceName) == 0) {
            // If already registered
            return returnedUserData;
        }
    }
    if (returnedUserData.getPwd().compare("") == 0) {
        // First time user. Set pwd and date.
        userData.setPwd(std::to_string(std::hash<std::string>{}(userData.getUid() + std::to_string(std::time(0)) + std::to_string(std::rand()))));
        userData.setDate((long int)std::time(0));
    }
    userData.addAuthorizedService(serviceName);
    writeUserData(userData);
    return userData;
}
