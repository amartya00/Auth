#include "DataStructures.h"
#include "Utils.h"

#include <string>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

void Sigsegv::Auth::WhiteListedUser::setUid(const std::string& uid) {
    this->uid = uid;
}

void Sigsegv::Auth::WhiteListedUser::setService(const std::string& service) {
    this->service = service;
}

std::string Sigsegv::Auth::WhiteListedUser::getUid() const {
    return uid;
}

std::string Sigsegv::Auth::WhiteListedUser::getService() const {
    return service;
}

void Sigsegv::Auth::WhiteListedUser::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("u") != attrMap.end()) {
        uid = Sigsegv::Utils::toStdString(attrMap.at("u").GetS());
    }
    if (attrMap.find("s") != attrMap.end()) {
        service = Sigsegv::Utils::toStdString(attrMap.at("s").GetS());
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::WhiteListedUser::toAttributeMap () const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    }
    if (service.compare("") != 0) {
        retval["s"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(service));
    }
    return retval;
}

std::string Sigsegv::Auth::WhiteListedUser::toKeyConditionExpression() const {
    std::vector<std::string> expressions;
    if (uid.compare("") != 0) {
        expressions.push_back("u = :u");
    }
    if (service.compare("") != 0) {
        expressions.push_back("s = :s");
    }
    std::string retval = "";
    for (unsigned int i = 0; i < expressions.size() - 1; i++) {
        retval += expressions[i] + " and ";
    }
    retval += expressions[expressions.size() - 1];
    return retval;
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::WhiteListedUser::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval[":u"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    }
    if (service.compare("") != 0) {
        retval[":s"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(service));
    }
    return retval;
}

