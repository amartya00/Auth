#include"GapiWrapper.h"
#include "Exceptions.h"
#include "Utils.h"

#include <json/json.h>
#include <curl/curl.h>

#include <sstream>
#include <vector>

size_t Sigsegv::Auth::GapiWrapper::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    ((std::string*)userp)->append((char*)contents, realsize);
    return realsize;
}

Sigsegv::Auth::UserItem Sigsegv::Auth::GapiWrapper::getInfoFromOAuth(const std::string &accessToken) const {
    std::string url = "https://www.googleapis.com/plus/v1/people/me?access_token=" + accessToken;
    Sigsegv::Auth::Exceptions::ServiceException exc;
    Sigsegv::Auth::Exceptions::ClientException cexc;
    std::string returned;

    // Set up and make calls with curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_INIT_ERROR);
        exc.setMessage("Failed to initialize curl.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    auto response = curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_URL);
        exc.setMessage("Failed to set curl url.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION);
        exc.setMessage("Failed to set curl follow location.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION);
        exc.setMessage("Failed to set curl writeback function.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&returned);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_SETOPT_ERROR_CURLOPT_WRITEDATA);
        exc.setMessage("Failed to set curl writeback data.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    response = curl_easy_perform(curl);
    if (response != CURLE_OK) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::CURL_PERFORM_ERROR);
        exc.setMessage("Failed to call curl.");
        curl_easy_cleanup(curl);
        throw exc;
    }
    curl_easy_cleanup(curl);

    // Parse response into json object
    std::istringstream ifs(returned);
    Json::Value responseJson;
    ifs >> responseJson;

    // Check for errors
    if (responseJson["error"].size() > 0) {
        if (responseJson["error"]["code"].asInt() == 403) {
            cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::INVALID_TOKEN);
            cexc.setMessage("(403) " + responseJson["error"]["message"].asString());
            throw cexc;
        }
        if (responseJson["error"]["code"].asInt() == 401) {
            cexc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::EXPIRED_TOKEN);
            cexc.setMessage("(401) " + responseJson["error"]["message"].asString());
            throw cexc;
        }
    }
    if (responseJson["name"]["givenName"].asString().compare("") == 0 ||
            responseJson["name"]["familyName"].asString().compare("") == 0 ||
            responseJson["emails"][0]["value"].asString().compare("") == 0) {
        exc.setErrorCode(Sigsegv::Auth::Exceptions::ExceptionType::OAUTH_FAIL);
        exc.setMessage("Failed to get user info from oauth.");
        throw exc;
    }

    // If no errors, construct and return AuthUserItem object
    Sigsegv::Auth::UserItem user;
    user.setName(responseJson["name"]["givenName"].asString() + " "  + responseJson["name"]["familyName"].asString());
    user.setUid(responseJson["emails"][0]["value"].asString());
    return user;
}
