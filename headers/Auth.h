#ifndef AUTH_H
#define AUTH_H

#include <istream>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

namespace Auth {
    class Service {
    private:
        std::shared_ptr<Aws::DynamoDB::DynamoDBClient> ddbClient;
        std::string whitelistedServicesTable;
        std::string whitelistedUsersTable;
        std::string authTable;
    public:
        Service(const std::shared_ptr<Aws::DynamoDB::DynamoDBClient>& ddbClient, const std::istream& configStream);
        virtual ~Service();

    };
}

#endif
