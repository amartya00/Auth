#include "DataStructures.h"
#include "Utils.h"

#include <set>
#include <string>
#include <sstream>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

void Sigsegv::Auth::UserItem::setName(const std::string& name) {
    this->name = name;
}

void Sigsegv::Auth::UserItem::setUid(const std::string& uid) {
    this->uid = uid;
}

void Sigsegv::Auth::UserItem::setPwd(const std::string& pwd) {
    this->pwd = pwd;
}

void Sigsegv::Auth::UserItem::addAuthorizedService(const std::string& service) {
    authorizedServices.insert(service);
}

void Sigsegv::Auth::UserItem::removeAuthorizedService(const std::string & service){
    auto it = authorizedServices.find(service);
    if (it != authorizedServices.end()) {
        authorizedServices.erase(it);
    }
}

void Sigsegv::Auth::UserItem::setDate(const long date) {
    this->date = date;
}

std::string Sigsegv::Auth::UserItem::getName() const {
    return name;
}

std::string Sigsegv::Auth::UserItem::getUid() const {
    return uid;
}

std::string Sigsegv::Auth::UserItem::getPwd() const {
    return pwd;
}

std::set<std::string> Sigsegv::Auth::UserItem::getAuthorizedServices() const {
    return authorizedServices;
}

long int Sigsegv::Auth::UserItem::getDate() const {
    return date;
}

void Sigsegv::Auth::UserItem::fromAttributeMap(const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap) {
    if (attrMap.find("u") != attrMap.end()) {
        uid = Sigsegv::Utils::toStdString(attrMap.at("u").GetS());
    }
    if (attrMap.find("p") != attrMap.end()) {
        pwd = Sigsegv::Utils::toStdString(attrMap.at("p").GetS());
    }
    if (attrMap.find("n") != attrMap.end()) {
        name = Sigsegv::Utils::toStdString(attrMap.at("n").GetS());
    }
    if (attrMap.find("a") != attrMap.end()) {
        std::string servicesStr {Sigsegv::Utils::toStdString(attrMap.at("n").GetS())};
        std::istringstream iss {servicesStr};
        std::string word;
        while(std::getline(iss, word, ',')) {
            if (word.compare("") != 0) {
                authorizedServices.insert(word);
            }
        }
    }
    if (attrMap.find("d") != attrMap.end()) {
        date = std::stol(Sigsegv::Utils::toStdString(attrMap.at("d").GetN()));
    }
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::UserItem::toAttributeMap () const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval["u"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval["p"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(pwd));
    }
    if (name.compare("") != 0) {
        retval["n"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(name));
    }
    if (authorizedServices.size() != 0) {
        std::string servicesStr {""};
        for (auto s : authorizedServices) {
            servicesStr += (s + ",");
        }
        retval["a"] = Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(servicesStr));
    }
    if (date != -1) {
        retval["d"] = Aws::DynamoDB::Model::AttributeValue().SetN(Sigsegv::Utils::toAwsString(std::to_string(date)));
    }
    return retval;
}

std::string Sigsegv::Auth::UserItem::toKeyConditionExpression() const {
    std::vector<std::string> expressions;
    if (uid.compare("") != 0) {
        expressions.push_back("u = :u");
    }
    if (pwd.compare("") != 0) {
        expressions.push_back("p = :p");
    }
    std::string retval = "";
    for (unsigned int i = 0; i < expressions.size() - 1; i++) {
        retval += expressions[i] + " and ";
    }
    retval += expressions[expressions.size() - 1];
    return retval;
}

Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> Sigsegv::Auth::UserItem::toKeyConditionAttributeValues() const {
    Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> retval;
    if (uid.compare("") != 0) {
        retval[":u"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(uid));
    }
    if (pwd.compare("") != 0) {
        retval[":p"] =  Aws::DynamoDB::Model::AttributeValue().SetS(Sigsegv::Utils::toAwsString(pwd));
    }
    return retval;
}
