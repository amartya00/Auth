#include "Mocks.h"

Sigsegv::Auth::UserItem Sigsegv::Auth::Mocks::MockGapiWrapper::getInfoFromOAuth(const std::string &accessToken) const {
    return response;
}

void Sigsegv::Auth::Mocks::MockGapiWrapper::setResponse(const Sigsegv::Auth::UserItem& response) {
    this->response = response;
}
