#ifndef AUTH_DS_H
#define AUTH_DS_H

#include <string>
#include <set>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

namespace Sigsegv {
namespace Auth {
    class UserItem {
    private:
        std::string uid; // Hash
        std::string pwd; // Range
        std::string name;
        std::set<std::string> authorizedServices;
        long int date;
    public:
        // Constructors and destructors
        UserItem() : uid {""}, pwd {""}, name {""}, authorizedServices {}, date {-1} {}
        UserItem(const UserItem& other) : uid {other.uid}, pwd {other.pwd}, name {other.name}, authorizedServices {other.authorizedServices}, date {other.date} {}
        virtual ~UserItem() {}

        // Setters and getters
        void setUid(const std::string&);
        void setPwd(const std::string&);
        void setName(const std::string&);
        void addAuthorizedService(const std::string&);
        void removeAuthorizedService(const std::string&);
        void setDate(const long int date);

        std::string getUid() const;
        std::string getPwd() const;
        std::string getName() const;
        std::set<std::string> getAuthorizedServices() const;
        long int getDate() const;

        // DDB translations
        void fromAttributeMap (const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap);
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toAttributeMap () const;
        std::string toKeyConditionExpression () const;
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toKeyConditionAttributeValues () const;
    };

    class WhitelistEnabledService {
    private:
        std::string serviceName; // H
        bool isWhitelistingEnabled;
    public:
        // Constructors and destructors
        WhitelistEnabledService() : serviceName {""}, isWhitelistingEnabled {false} {}
        WhitelistEnabledService(const WhitelistEnabledService& other) : serviceName {other.serviceName}, isWhitelistingEnabled {other.isWhitelistingEnabled} {}
        virtual ~WhitelistEnabledService() {}

        // Setters and getters
        void setServiceName(const std::string& serviceName);
        void setWhitelisting(bool isWhitelistingEnabled);

        std::string getServiceName() const;
        bool getWhitelisting() const;

        // DDB translations
        void fromAttributeMap (const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap);
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toAttributeMap () const;
        std::string toKeyConditionExpression () const;
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toKeyConditionAttributeValues () const;
    };

    class WhiteListedUser {
    private:
        std::string uid; // H
        std::string service; // R
    public:
        // Constructors and destructors
        WhiteListedUser() : uid {""}, service {""} {}
        WhiteListedUser(const WhiteListedUser& other) : uid {other.uid}, service {other.service} {}
        virtual ~WhiteListedUser() {}

        // Setters and getters
        void setUid(const std::string& uid);
        void setService(const std::string& service);

        std::string getUid() const;
        std::string getService() const;

        // DDB translations
        void fromAttributeMap (const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap);
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toAttributeMap () const;
        std::string toKeyConditionExpression () const;
        Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toKeyConditionAttributeValues () const;
    };
}
}
#endif
