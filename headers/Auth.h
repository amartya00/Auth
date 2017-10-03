#ifndef AUTH_H
#define AUTH_H

#include <istream>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "DataStructures.h"
#include "GapiWrapper.h"

namespace Sigsegv {
    namespace Auth {
        class Service {
        private:
            std::shared_ptr<Aws::DynamoDB::DynamoDBClient> ddbClient;
            std::shared_ptr<Sigsegv::Auth::GapiWrapper> gapiWrapper;
            std::string whitelistedServicesTable;
            std::string whitelistedUsersTable;
            std::string authTable;

            // Make gapi calls for user info
            bool isWhitelistingEnabledForService(const std::string& serviceName) const;
            bool isUserWhitelisted(const Sigsegv::Auth::UserItem& user, const std::string& serviceName) const;
            Sigsegv::Auth::UserItem isUserRegisteredForService(const Sigsegv::Auth::UserItem& user, const std::string& service) const;
            void writeUserData(const Sigsegv::Auth::UserItem& user) const;

        public:
            Service(const std::shared_ptr<Aws::DynamoDB::DynamoDBClient>& ddbClient, std::istream& configStream, const std::shared_ptr<Sigsegv::Auth::GapiWrapper>& gapiWrapper);
            virtual ~Service() {}
            Sigsegv::Auth::UserItem signinOrRegister(const std::string& oauthToken, const std::string& serviceName) const;
            bool authorizeUser(const std::string& uid, const std::string& pwd, const std::string& serviceName) const;
        };
    }
}

#endif
