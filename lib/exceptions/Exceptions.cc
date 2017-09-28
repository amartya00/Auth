#include "Exceptions.h"

void Sigsegv::Auth::Exceptions::ServiceException::operator =(const Sigsegv::Auth::Exceptions::ServiceException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* Sigsegv::Auth::Exceptions::ServiceException::what() const noexcept {
    return message.c_str();
}

void Sigsegv::Auth::Exceptions::ServiceException::setMessage(const std::string& message) {
    this->message = message;
}

void Sigsegv::Auth::Exceptions::ServiceException::setErrorCode(const Sigsegv::Auth::Exceptions::ExceptionType errorCode) {
    this->errorCode= errorCode;
}

std::string Sigsegv::Auth::Exceptions::ServiceException::getMessage() const {
    return message;
}

Sigsegv::Auth::Exceptions::ExceptionType Sigsegv::Auth::Exceptions::ServiceException::getErrorCode() const {
    return errorCode;
}



void Sigsegv::Auth::Exceptions::ClientException::operator =(const Sigsegv::Auth::Exceptions::ClientException& other) {
    this->message = other.message;
    this->errorCode = other.errorCode;
}

const char* Sigsegv::Auth::Exceptions::ClientException::what() const noexcept {
    return message.c_str();
}

void Sigsegv::Auth::Exceptions::ClientException::setMessage(const std::string& message) {
    this->message = message;
}

void Sigsegv::Auth::Exceptions::ClientException::setErrorCode(const Sigsegv::Auth::Exceptions::ExceptionType errorCode) {
    this->errorCode= errorCode;
}

std::string Sigsegv::Auth::Exceptions::ClientException::getMessage() const {
    return message;
}

Sigsegv::Auth::Exceptions::ExceptionType Sigsegv::Auth::Exceptions::ClientException::getErrorCode() const {
    return errorCode;
}
