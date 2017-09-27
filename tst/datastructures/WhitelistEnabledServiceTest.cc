#define CATCH_CONFIG_MAIN

#include "catch.hpp"

#include "DataStructures.h"

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

SCENARIO ("Happy case for WhitelistEnabledService", "[WhitelistEnabledServiceTest]") {

    GIVEN ("I have a empty (default) WhitelistEnabledService") {
        Sigsegv::Auth::WhitelistEnabledService item;

        WHEN ("I do gets on this") {

            THEN ("I should get back default values") {

                REQUIRE(item.getServiceName().compare("") == 0);
                REQUIRE(!item.getWhitelisting());
            }
        }

        WHEN ("I set values to the parameters") {
            item.setServiceName("S1");
            item.setWhitelisting(true);

            THEN ("I should get back those values") {

                REQUIRE(item.getServiceName().compare("S1") == 0);
                REQUIRE(item.getWhitelisting());
            }
        }
    }
}

SCENARIO ("Test WhitelistEnabledService to ddb", "[WhitelistEnabledServiceTest]") {

    GIVEN ("I have set up a user item with required fields") {

        Sigsegv::Auth::WhitelistEnabledService item;
        item.setServiceName("S1");
        item.setWhitelisting(true);

        WHEN ("I convert this to a ddb attribute map") {

            auto actual = item.toAttributeMap();
            auto keyConditionExpression = item.toKeyConditionExpression();
            auto keyConditionAttrValues = item.toKeyConditionAttributeValues();

            THEN ("The generated attribute map to match what I expect") {

                REQUIRE(actual.at("s").GetS().compare("S1") == 0);
            }

            THEN ("The generated key condition expression should match the expected") {

                REQUIRE(keyConditionExpression.compare("s = :s "));
            }

            THEN ("The key condition attribute values should match what's expected") {

                REQUIRE(keyConditionAttrValues[":s"].GetS().compare("S1") == 0);
            }
        }
    }
}
