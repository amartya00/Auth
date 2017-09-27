#include "DataStructures.h"
#include "Utils.h"

#include <string>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

void Sigsegv::Auth::WhitelistEnabledService::setServiceName(const std::string& serviceName) {
    this->serviceName = serviceName;
}

void Sigsegv::Auth::WhitelistEnabledService::setWhitelisting(bool isWhitelistingEnabled) {
    this->isWhitelistingEnabled = isWhitelistingEnabled;
}

std::string Sigsegv::Auth::WhitelistEnabledService::getServiceName() const {
    return serviceName;
}

bool Sigsegv::Auth::WhitelistEnabledService::getWhitelisting() const {
    return isWhitelistingEnabled;
}

void Sigsegv::Auth::WhitelistEnabledService::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("s") != attrMap.end()) {
        serviceName = Sigsegv::Utils::toStdString(attrMap.at("s").GetS());
    }
    if (attrMap.find("i") != attrMap.end()) {
        std::string temp = Sigsegv::Utils::toStdString(attrMap.at("i").GetS());
        if (temp.compare("True") == 0) {
            isWhitelistingEnabled = true;
        } else {
            isWhitelistingEnabled = false;
        }
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::WhitelistEnabledService::toAttributeMap () const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (serviceName.compare("") != 0) {
        retval["s"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(serviceName));
    }
    if (isWhitelistingEnabled) {
        retval["i"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString("True"));
    } else {
        retval["i"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString("False"));
    }
    return retval;
}

std::string Sigsegv::Auth::WhitelistEnabledService::toKeyConditionExpression() const {
    std::vector<std::string> expressions;
    if (serviceName.compare("") != 0) {
        expressions.push_back("s = :s");
    }
    std::string retval = "";
    for (unsigned int i = 0; i < expressions.size() - 1; i++) {
        retval += expressions[i] + " and ";
    }
    retval += expressions[expressions.size() - 1];
    return retval;
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::WhitelistEnabledService::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (serviceName.compare("") != 0) {
        retval[":s"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(serviceName));
    }
    return retval;
}

