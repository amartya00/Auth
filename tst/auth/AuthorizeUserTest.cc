#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "Auth.h"
#include "Mocks.h"
#include "Exceptions.h"

#include <string>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>

SCENARIO ("An existing user is authorized for a certain operation") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up my fake ddb client to return appropriate user data") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        std::set<std::string> services {"S1"};
        client->setServiceWhitelistingResponse("S1", false);
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);
        client->setContext(context);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I call AuthorizeUser") {

            auto response = service.authorizeUser("user@gmail.com", "1234", "S1");

            THEN ("The the service whitelisting qury should be called with proper request") {

                auto expression = client->getContext()->serviceWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->serviceWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("s = :s") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("S1") == 0);
                REQUIRE(attributeValues.size() == 1);
            }

            THEN ("The user whitelisting table should be not be queried at all") {

                auto expression = client->getContext()->userWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("") == 0);
                REQUIRE(attributeValues.size() == 0);
            }

            THEN ("The user table should be queried with correct parameters") {

                auto expression = client->getContext()->userQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("u = :u and p = :p") == 0);
                REQUIRE(attributeValues.at(":u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(attributeValues.at(":p").GetS().compare("1234") == 0);
                REQUIRE(attributeValues.size() == 2);
            }

            THEN ("I should be authorized for the operation") {

                REQUIRE(response);
            }
        }
    }
}

SCENARIO ("A whitelisting enabled service has the user whitelisted") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up my fake ddb client to return appropriate user data") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        std::set<std::string> services {"S1"};
        client->setServiceWhitelistingResponse("S1", true);
        client->setQueryWhitelistedUserResponse("user@gmail.com", "S1");
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);
        client->setContext(context);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I call AuthorizeUser") {

            auto response = service.authorizeUser("user@gmail.com", "1234", "S1");

            THEN ("The the service whitelisting qury should be called with proper request") {

                auto expression = client->getContext()->serviceWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->serviceWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("s = :s") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("S1") == 0);
                REQUIRE(attributeValues.size() == 1);
            }

            THEN ("The user whitelisting table should be queried qith correct parameters") {

                auto expression = client->getContext()->userWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("u = :u and s = :s") == 0);
                REQUIRE(attributeValues.at(":u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("S1") == 0);
                REQUIRE(attributeValues.size() == 2);
            }

            THEN ("The user table should be queried with correct parameters") {

                auto expression = client->getContext()->userQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("u = :u and p = :p") == 0);
                REQUIRE(attributeValues.at(":u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(attributeValues.at(":p").GetS().compare("1234") == 0);
                REQUIRE(attributeValues.size() == 2);
            }

            THEN ("I should be authorized for the operation") {

                REQUIRE(response);
            }
        }
    }
}

SCENARIO ("A non whitelisted user tries to access a whitelisting enabled service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up my fake ddb client to return appropriate user data") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        std::set<std::string> services {"S1"};
        client->setServiceWhitelistingResponse("S1", true);
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);
        client->setContext(context);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I call AuthorizeUser") {

            auto response = service.authorizeUser("user@gmail.com", "1234", "S1");

            THEN ("The the service whitelisting qury should be called with proper request") {

                auto expression = client->getContext()->serviceWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->serviceWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("s = :s") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("S1") == 0);
                REQUIRE(attributeValues.size() == 1);
            }

            THEN ("The user whitelisting table should be queried qith correct parameters") {

                auto expression = client->getContext()->userWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("u = :u and s = :s") == 0);
                REQUIRE(attributeValues.at(":u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("S1") == 0);
                REQUIRE(attributeValues.size() == 2);
            }

            THEN ("The user table should not be queried at all") {

                auto expression = client->getContext()->userQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("") == 0);
                REQUIRE(attributeValues.size() == 0);
            }

            THEN ("I should be authorized for the operation") {

                REQUIRE(!response);
            }
        }
    }
}

SCENARIO ("An existing user does not have authority to access a particular service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up my fake ddb client to return appropriate user data") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        std::set<std::string> services {"S1"};
        client->setServiceWhitelistingResponse("S1", false);
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);
        client->setContext(context);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I call AuthorizeUser") {

            auto response = service.authorizeUser("user@gmail.com", "1234", "NOT_S1");

            THEN ("The the service whitelisting qury should be called with proper request") {

                auto expression = client->getContext()->serviceWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->serviceWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("s = :s") == 0);
                REQUIRE(attributeValues.at(":s").GetS().compare("NOT_S1") == 0);
                REQUIRE(attributeValues.size() == 1);
            }

            THEN ("The user whitelisting table should not be queried at all") {

                auto expression = client->getContext()->userWhitelistQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userWhitelistQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("") == 0);
                REQUIRE(attributeValues.size() == 0);
            }

            THEN ("The user table should be queried with correct parameters") {

                auto expression = client->getContext()->userQueryItemRequest.GetKeyConditionExpression();
                auto attributeValues = client->getContext()->userQueryItemRequest.GetExpressionAttributeValues();
                REQUIRE(expression.compare("u = :u and p = :p") == 0);
                REQUIRE(attributeValues.at(":u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(attributeValues.at(":p").GetS().compare("1234") == 0);
                REQUIRE(attributeValues.size() == 2);
            }

            THEN ("I should be authorized for the operation") {

                REQUIRE(!response);
            }
        }
    }
}
