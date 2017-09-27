#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "DataStructures.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

SCENARIO ("Happy case for WhiteListedUser", "[WhiteListedUserTest]") {

    GIVEN ("I have a empty (default) WhiteListedUser") {
        Sigsegv::Auth::WhiteListedUser item;

        WHEN ("I do gets on this") {

            THEN ("I should get back default values") {

                REQUIRE(item.getUid().compare("") == 0);
                REQUIRE(item.getService().compare("") == 0);
            }
        }

        WHEN ("I set values to the parameters") {
            item.setUid("UID");
            item.setService("S1");

            THEN ("I should get back those values") {

                REQUIRE(item.getUid().compare("UID") == 0);
                REQUIRE(item.getService().compare("S1") == 0);
            }
        }
    }
}

SCENARIO ("Test WhiteListedUser to ddb", "[WhiteListedUserTest]") {

    GIVEN ("I have set up a user item with required fields") {

        Sigsegv::Auth::WhiteListedUser item;
        item.setUid("UID");
        item.setService("S1");

        WHEN ("I convert this to a ddb attribute map") {

            auto actual = item.toAttributeMap();
            auto keyConditionExpression = item.toKeyConditionExpression();
            auto keyConditionAttrValues = item.toKeyConditionAttributeValues();

            THEN ("The generated attribute map to match what I expect") {

                REQUIRE(actual.at("u").GetS().compare("UID") == 0);
                REQUIRE(actual.at("s").GetS().compare("S1") == 0);
            }

            THEN ("The generated key condition expression should match the expected") {

                REQUIRE(keyConditionExpression.compare("u = :u and s = :s "));
            }

            THEN ("The key condition attribute values should match what's expected") {

                REQUIRE(keyConditionAttrValues[":u"].GetS().compare("UID") == 0);
                REQUIRE(keyConditionAttrValues[":s"].GetS().compare("S1") == 0);
            }
        }
    }
}
