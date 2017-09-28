#include "Auth.h"
#include "DataStructures.h"
#include "Utils.h"
#include "Exceptions.h"

#include <vector>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

Sigsegv::Auth::UserItem Sigsegv::Auth::Service::IsUserRegisteredForService(const Sigsegv::Auth::UserItem &user, const std::string &service) const {
    Sigsegv::Auth::UserItem item;
    item.setUid(user.getPwd());
    item.setPwd(user.getPwd());

    auto result = ddbClient->Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(Sigsegv::Utils::toAwsString(authTable))
                    .WithKeyConditionExpression(Sigsegv::Utils::toAwsString(item.toKeyConditionExpression()))
                    .WithExpressionAttributeValues(item.toKeyConditionAttributeValues()));

    if (!result.IsSuccess()) {
        Sigsegv::Auth::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::DDB_QUERY_FAIL);
        exc.setMessage(Sigsegv::Utils::toStdString(result.GetError().GetMessage()));
        throw exc;
    }

    std::vector<Sigsegv::Auth::UserItem> useritems;
    auto items = result.GetResult().GetItems();
    for (auto returnedItem : items) {
        Sigsegv::Auth::UserItem temp;
        temp.fromAttributeMap(returnedItem);
        useritems.push_back(temp);
    }

    // Use the one with the greatest timestamp
    Sigsegv::Auth::UserItem temp;
    long int greatestTimestamp {-1};
    for (auto returnedItem : useritems) {
        if (returnedItem.getDate() > greatestTimestamp) {
            temp = returnedItem;
        }
    }
    return temp;
}
