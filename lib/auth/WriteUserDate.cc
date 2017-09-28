#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>

void Sigsegv::Auth::Service::writeUserData(const Sigsegv::Auth::UserItem &user) const {
    Sigsegv::Auth::Exceptions::ClientException cexc;
    if (user.getUid().compare("") == 0) {
        cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Missing uid in user item");
        throw cexc;
    }
    if (user.getPwd().compare("") == 0) {
        cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Missing pwd in user item");
        throw cexc;
    }
    if (user.getName().compare("") == 0) {
        cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Missing name in user item");
        throw cexc;
    }
    if (user.getDate() <=  0) {
        cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("Invalid date");
        throw cexc;
    }
    if (user.getAuthorizedServices().size() == 0) {
        cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_INPUT);
        cexc.setMessage("No authorized services.");
        throw cexc;
    }
    auto response = ddbClient->PutItem(
                Aws::DynamoDB::Model::PutItemRequest()
                    .WithTableName(Sigsegv::Utils::toAwsString(authTable))
                    .WithItem(user.toAttributeMap()));
    if (!response.IsSuccess()) {
        Sigsegv::Auth::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::DDB_PUT_ITEM_FAIL);
        exc.setMessage(Sigsegv::Utils::toStdString(response.GetError().GetMessage()));
        throw exc;
    }
}
