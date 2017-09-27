#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "DataStructures.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

SCENARIO ("Happy case for UserItem", "[UserItemTest]") {

    GIVEN ("I have a empty (default) UserItem") {
        Sigsegv::Auth::UserItem item;

        WHEN ("I do gets on this") {

            THEN ("I should get back default values") {

                REQUIRE(item.getUid().compare("") == 0);
                REQUIRE(item.getPwd().compare("") == 0);
                REQUIRE(item.getName().compare("") == 0);
                REQUIRE(item.getAuthorizedServices().size() == 0);
            }
        }

        WHEN ("I set values to the parameters") {
            item.setUid("UID");
            item.setPwd("PWD");
            item.setName("NAME");
            item.addAuthorizedService("S1");
            item.addAuthorizedService("S1");
            item.addAuthorizedService("S2");

            THEN ("I should get back those values") {
                REQUIRE(item.getUid().compare("UID") == 0);
                REQUIRE(item.getPwd().compare("PWD") == 0);
                REQUIRE(item.getName().compare("NAME") == 0);
                REQUIRE(item.getAuthorizedServices().size() == 2);
                REQUIRE(item.getAuthorizedServices().find("S1") != item.getAuthorizedServices().end());
                REQUIRE(item.getAuthorizedServices().find("S2") != item.getAuthorizedServices().end());
            }
        }
    }
}

SCENARIO ("Test UserItem to ddb", "[UserItemTest]") {
    GIVEN ("I have set up a user item with required fields") {

        Sigsegv::Auth::UserItem item;
        item.setUid("UID");
        item.setPwd("PWD");
        item.setName("NAME");
        item.addAuthorizedService("S1");
        item.addAuthorizedService("S2");

        WHEN ("I convert this to a ddb attribute map") {

            auto actual = item.toAttributeMap();
            auto keyConditionExpression = item.toKeyConditionExpression();
            auto keyConditionAttrValues = item.toKeyConditionAttributeValues();

            THEN ("The generated attribute map to match what I expect") {

                REQUIRE(actual.at("u").GetS().compare("UID") == 0);
                REQUIRE(actual.at("p").GetS().compare("PWD") == 0);
                REQUIRE(actual.at("n").GetS().compare("NAME") == 0);
                REQUIRE(actual.at("a").GetS().compare("S1,S2,") == 0);
            }

            THEN ("The generated key condition expression should match the expected") {

                REQUIRE(keyConditionExpression.compare("u = :u and p = :p "));
            }

            THEN ("The key condition attribute values should match what's expected") {

                REQUIRE(keyConditionAttrValues[":u"].GetS().compare("UID") == 0);
                REQUIRE(keyConditionAttrValues[":p"].GetS().compare("PWD") == 0);
            }
        }
    }
}
