#include "Mocks.h"
#include "Utils.h"

#include <string>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

const std::string Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE {"AuthUserTable"};
const std::string Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE {"ServiceWhitelistingTable"};
const std::string Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE {"WhitelistedUsersTable"};

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setSucceedOnQuery(bool val) {
    succeedOnQuery = val;
}

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setSucceedOnPutItem(bool val) {
    succeedOnPutItem = val;
}

bool Sigsegv::Auth::Mocks::MockDynamoDBClient::getSucceedOnQuery() const{
    return succeedOnQuery;
}

bool Sigsegv::Auth::Mocks::MockDynamoDBClient::getSucceedOnPutItem() const{
    return succeedOnPutItem;
}

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setQueryUserItemResponse(
        const std::string &uid,
        const std::string &pwd,
        const std::string &name,
        const long date,
        const std::set<std::string> &authorizedServices) {
    queryUserItemResponse["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    queryUserItemResponse["p"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(pwd));
    queryUserItemResponse["n"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(name));
    queryUserItemResponse["d"] = Aws::DynamoDB::Model::AttributeValue().SetN(Sigsegv::Utils::toAwsString(std::to_string(date)));
    std::string servicesStr {""};
    for (auto s : authorizedServices) {
        servicesStr += (s + ",");
    }
    queryUserItemResponse["a"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(servicesStr));
}

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setQueryWhitelistedUserResponse(const std::string& uid, const std::string& service) {
    queryWhitelistedUserResponse["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    queryWhitelistedUserResponse["s"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(service));
}

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setServiceWhitelistingResponse(const std::string& service, const bool isWhitelisted) {
    queryServiceWhitelistingResponse["i"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString((isWhitelisted? "True" : "False")));
    queryServiceWhitelistingResponse["s"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(service));
}

Aws::DynamoDB::Model::QueryOutcome Sigsegv::Auth::Mocks::MockDynamoDBClient::Query(const Aws::DynamoDB::Model::QueryRequest &request) const {
    Aws::Client::AWSError<Aws::DynamoDB::DynamoDBErrors> error;
    Aws::DynamoDB::Model::QueryResult result;
    Aws::Vector<Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>> items;

    if (request.GetTableName().compare(AUTH_USER_TABLE) == 0) {
        if (queryUserItemResponse.size() != 0) {
            items.push_back(queryUserItemResponse);
        }
    } else if (request.GetTableName().compare(SERV_WHITELISTING_TABLE) == 0) {
        if (queryServiceWhitelistingResponse.size() != 0) {
            items.push_back(queryServiceWhitelistingResponse);
        }
    } else {
        if (queryWhitelistedUserResponse.size() != 0) {
            items.push_back(queryWhitelistedUserResponse);
        }
    }

    result.SetItems(items);

    if (getSucceedOnQuery()) {
        Aws::DynamoDB::Model::QueryOutcome retval(result);
        return retval;
    } else {
        Aws::DynamoDB::Model::QueryOutcome retval(error);
        return retval;
    }
}

Aws::DynamoDB::Model::PutItemOutcome Sigsegv::Auth::Mocks::MockDynamoDBClient::PutItem(const Aws::DynamoDB::Model::PutItemRequest &request) const {
    std::cout << "Mock put item\n";
    Aws::DynamoDB::Model::PutItemResult result;
    Aws::Client::AWSError<Aws::DynamoDB::DynamoDBErrors> error;
    this->context->putItemRequest = request;
    if (getSucceedOnQuery()) {
        Aws::DynamoDB::Model::PutItemOutcome retval(result);
        return retval;
    } else {
        Aws::DynamoDB::Model::PutItemOutcome retval(error);
        return retval;
    }
}

void Sigsegv::Auth::Mocks::MockDynamoDBClient::setContext(const std::shared_ptr<Context>& context) {
    this->context = context;
}

const std::shared_ptr<Sigsegv::Auth::Mocks::Context> Sigsegv::Auth::Mocks::MockDynamoDBClient::getContext() const {
    return context;
}
