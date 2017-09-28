#include "Auth.h"
#include "DataStructures.h"
#include "Utils.h"
#include "Exceptions.h"

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

bool Sigsegv::Auth::Service::isWhitelistingEnabledForService(const std::string &serviceName) const {
    Sigsegv::Auth::WhitelistEnabledService item;
    item.setServiceName(serviceName);

    auto result = ddbClient->Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(Sigsegv::Utils::toAwsString(whitelistedServicesTable))
                    .WithKeyConditionExpression(Sigsegv::Utils::toAwsString(item.toKeyConditionExpression()))
                    .WithExpressionAttributeValues(item.toKeyConditionAttributeValues()));
    if (!result.IsSuccess()) {
        Sigsegv::Auth::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::DDB_QUERY_FAIL);
        exc.setMessage(Sigsegv::Utils::toStdString(result.GetError().GetMessage()));
        throw exc;
    }
    auto items = result.GetResult().GetItems();
    if (items.size() == 0) {
        Sigsegv::Auth::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_SERVICE);
        exc.setMessage("Invalid service");
        throw exc;
    }
    item.fromAttributeMap(items[0]);
    return item.getWhitelisting();
}
