#ifndef SIGSEGV_AUTH_MOCKS
#define SIGSEGV_AUTH_MOCKS

#include <set>
#include <string>
#include <vector>

#include <curl/curl.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>
#include <aws/dynamodb/model/PutItemRequest.h>

#include "GapiWrapper.h"
#include "DataStructures.h"

namespace Sigsegv {
    namespace Auth {
        namespace Mocks {
            class Context {
            public:
                Aws::DynamoDB::Model::PutItemRequest putItemRequest;
                Aws::DynamoDB::Model::QueryRequest userQueryItemRequest;
                Aws::DynamoDB::Model::QueryRequest serviceWhitelistQueryItemRequest;
                Aws::DynamoDB::Model::QueryRequest userWhitelistQueryItemRequest;
            };

            class MockGapiWrapper : public GapiWrapper {
            private:
                Sigsegv::Auth::UserItem response;
            public:
                void setResponse(const Sigsegv::Auth::UserItem& response);
                Sigsegv::Auth::UserItem getInfoFromOAuth(const std::string &accessToken) const;
            };

            class MockDynamoDBClient : public Aws::DynamoDB::DynamoDBClient {
            private:
                std::vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> queryUserItemResponse;
                std::vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> queryWhitelistedUserResponse;
                std::vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> queryServiceWhitelistingResponse;

                bool succeedOnQuery {true};
                bool succeedOnPutItem {true};

                std::shared_ptr<Context> context;
            public:
                const static std::string AUTH_USER_TABLE;
                const static std::string SERV_WHITELISTING_TABLE;
                const static std::string WHITELISTED_USERS_TABLE;

                virtual Aws::DynamoDB::Model::QueryOutcome Query(const Aws::DynamoDB::Model::QueryRequest &request) const;
                virtual Aws::DynamoDB::Model::PutItemOutcome PutItem (const Aws::DynamoDB::Model::PutItemRequest &request) const;

                void setSucceedOnQuery(bool val);
                void setSucceedOnPutItem(bool val);
                bool getSucceedOnQuery() const;
                bool getSucceedOnPutItem() const;
                const std::shared_ptr<Context> getContext() const;

                void setQueryUserItemResponse(const std::string& uid, const std::string& pwd, const std::string& name, const long int date, const std::set<std::string>& authorizedServices);
                void setQueryWhitelistedUserResponse(const std::string& uid, const std::string& service);
                void setServiceWhitelistingResponse(const std::string& service, const bool isWhitelisted);

                void addQueryUserItemResponse(const std::string& uid, const std::string& pwd, const std::string& name, const long int date, const std::set<std::string>& authorizedServices);
                void addQueryWhitelistedUserResponse(const std::string& uid, const std::string& service);
                void addServiceWhitelistingResponse(const std::string& service, const bool isWhitelisted);

                void setContext(const std::shared_ptr<Context>& context);
            };
        }
    }
}

#endif
