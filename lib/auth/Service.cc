#include "Auth.h"
#include "Exceptions.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include <iostream>
#include <istream>

#include <json/json.h>

Sigsegv::Auth::Service::Service(const std::shared_ptr<Aws::DynamoDB::DynamoDBClient> &ddbClient, std::istream &configStream, const std::shared_ptr<Sigsegv::Auth::GapiWrapper>& gapiWrapper) {
    this->ddbClient = ddbClient;
    this->gapiWrapper = gapiWrapper;

    Json::Value jsonObj;
    configStream >> jsonObj;

    whitelistedServicesTable = jsonObj["WhitelistedServicesTable"].asString();
    whitelistedUsersTable = jsonObj["WhitelistedUsersTable"].asString();
    authTable = jsonObj["AuthTable"].asString();

    if (whitelistedServicesTable.compare("") == 0 ||
            whitelistedUsersTable.compare("") == 0 ||
            authTable.compare("") == 0) {
        Sigsegv::Auth::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_CONFIG);
        exc.setMessage("One of the table names not found in auth config");
        throw exc;
    }
}
