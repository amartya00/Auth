#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "Auth.h"
#include "Mocks.h"
#include "Exceptions.h"

#include <string>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>

SCENARIO ("An existing user tries to sign in to a non whitelisted service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up responses accordingly") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();

        Sigsegv::Auth::UserItem user;
        user.setUid("user@gmail.com");
        user.setName("USER");
        gapi->setResponse(user);

        std::set<std::string> services;
        services.insert("S1");
        client->setServiceWhitelistingResponse("S1", false);
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I try to sign in") {

            auto response = service.signinOrRegister("123", "S1");

            THEN ("I should get back my password and name and uid") {
                REQUIRE(response.getUid().compare("user@gmail.com") == 0);
                REQUIRE(response.getPwd().compare("1234") == 0);
                REQUIRE(response.getName().compare("USER") == 0);
                REQUIRE(response.getAuthorizedServices().size() == 1);
                REQUIRE(response.getAuthorizedServices().find("S1") != response.getAuthorizedServices().end());
            }
        }
    }

    Aws::ShutdownAPI(options);
}

SCENARIO ("An existing whitelisted user tries to sign in to a whitelisting enabled service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I have set up the responses accordingly") {
        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();

        Sigsegv::Auth::UserItem user;
        user.setUid("user@gmail.com");
        user.setName("USER");
        gapi->setResponse(user);

        std::set<std::string> services;
        services.insert("S1");
        client->setServiceWhitelistingResponse("S1", true);
        client->setQueryWhitelistedUserResponse("user@gmail.com", "S1");
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I try to sign in") {

            auto response = service.signinOrRegister("12345", "S1");

            THEN ("I should get back my uid and password") {
                REQUIRE(response.getUid().compare("user@gmail.com") == 0);
                REQUIRE(response.getPwd().compare("1234") == 0);
                REQUIRE(response.getUid().compare("user@gmail.com") == 0);
                REQUIRE(response.getName().compare("USER") == 0);
                REQUIRE(response.getAuthorizedServices().size() == 1);
                REQUIRE(response.getAuthorizedServices().find("S1") != response.getAuthorizedServices().end());
            }
        }
    }

    Aws::ShutdownAPI(options);
}

SCENARIO ("A non whitelisted user tries to sign in to a whitelisting enabled service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I have set up everything accordingly") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();

        Sigsegv::Auth::UserItem user;
        user.setUid("user@gmail.com");
        user.setName("USER");
        gapi->setResponse(user);

        std::set<std::string> services;
        services.insert("S1");
        client->setServiceWhitelistingResponse("S1", true);
        client->setQueryUserItemResponse("user@gmail.com", "1234", "USER", 100, services);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I try to sign in") {

            THEN ("I should ger a client exception") {

                REQUIRE_THROWS_WITH(service.signinOrRegister("1234", "S1"), Catch::Matchers::Contains("User not whitelisted in a whitelisting enabled service"));
            }
        }
    }

    Aws::ShutdownAPI(options);
}

SCENARIO ("User is whitelisted for service but has not yet registered") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN ("I set up everything correctly") {
        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        Sigsegv::Auth::UserItem user;
        user.setUid("user@gmail.com");
        user.setName("USER");
        gapi->setResponse(user);

        std::set<std::string> services;
        services.insert("S1");
        client->setServiceWhitelistingResponse("S1", true);
        client->setQueryWhitelistedUserResponse("user@gmail.com", "S1");
        client->setContext(context);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I sign in to the service") {

            auto result = service.signinOrRegister("12345", "S1");

            THEN ("I should get back the uid and passwred") {

                REQUIRE (result.getUid().compare("user@gmail.com") == 0);
                REQUIRE (result.getName().compare("USER") == 0);
            }

            THEN ("The ddb client should make a write request with my parameters") {
                auto item = client->getContext()->putItemRequest.GetItem();
                REQUIRE(item.at("u").GetS().compare("user@gmail.com") == 0);
                REQUIRE(item.at("p").GetS().compare(result.getPwd()) == 0);
                REQUIRE(item.at("a").GetS().compare("S1,") == 0);
                REQUIRE(item.at("n").GetS().compare("USER") == 0);
                REQUIRE(item.at("d").GetN().compare(std::to_string(result.getDate())) == 0);
            }
        }
    }

    Aws::ShutdownAPI(options);
}

SCENARIO ("I try to sign in to an invalid service") {

    Aws::SDKOptions options;
    Aws::InitAPI(options);

    GIVEN("I set up ddb client to return nothing from the services table") {

        auto gapi = std::make_shared<Sigsegv::Auth::Mocks::MockGapiWrapper>();
        auto client = std::make_shared<Sigsegv::Auth::Mocks::MockDynamoDBClient>();
        auto context = std::make_shared<Sigsegv::Auth::Mocks::Context>();

        Sigsegv::Auth::UserItem user;
        user.setUid("user@gmail.com");
        user.setName("USER");
        gapi->setResponse(user);

        std::string configjson {"{\n"};
        configjson += "\t\"WhitelistedServicesTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::SERV_WHITELISTING_TABLE + "\",\n";
        configjson += "\t\"WhitelistedUsersTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::WHITELISTED_USERS_TABLE + "\",\n";
        configjson += "\t\"AuthTable\": \"" + Sigsegv::Auth::Mocks::MockDynamoDBClient::AUTH_USER_TABLE + "\"\n";
        configjson += "}";

        std::istringstream iss {configjson};

        Sigsegv::Auth::Service service(client, iss, gapi);

        WHEN ("I try to sign in with the invalid service") {

            THEN ("There should be an invalid servce exception") {

                REQUIRE_THROWS_WITH(service.signinOrRegister("1234", "S1"), Catch::Matchers::Contains("Invalid service"));
            }
        }
    }
}
