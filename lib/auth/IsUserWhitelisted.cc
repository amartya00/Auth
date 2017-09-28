#include "Auth.h"
#include "DataStructures.h"
#include "Utils.h"
#include "Exceptions.h"

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

bool Sigsegv::Auth::Service::isUserWhitelisted(const Sigsegv::Auth::UserItem &user, const std::string &serviceName) const {
    Sigsegv::Auth::WhiteListedUser item;
    item.setService(serviceName);
    item.setUid(user.getUid());

    auto result = ddbClient->Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(Sigsegv::Utils::toAwsString(whitelistedUsersTable))
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
        return false;
    }
    return true;
}
