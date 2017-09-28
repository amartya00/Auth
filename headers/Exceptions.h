#ifndef SIGSEGV_AUTH_EXCEPTIONS
#define SIGSEGV_AUTH_EXCEPTIONS

#include <string>

namespace Sigsegv {
    namespace Auth {
        namespace Exceptions {
            enum class ExceptionType {
                UNKNOWN_EXCEPTION,
                INVALID_CONFIG,
                CURL_INIT_ERROR,
                CURL_SETOPT_ERROR_CURLOPT_URL,
                CURL_SETOPT_ERROR_CURLOPT_FOLLOWLOCATION,
                CURL_SETOPT_ERROR_CURLOPT_WRITEFUNCTION,
                CURL_SETOPT_ERROR_CURLOPT_WRITEDATA,
                INVALID_TOKEN,
                EXPIRED_TOKEN,
                INVALID_SERVICE,
                DDB_PUT_ITEM_FAIL,
                DDB_QUERY_FAIL,
                OAUTH_FAIL,
                CURL_PERFORM_ERROR,
                INVALID_INPUT,
                WHITELIST_ERROR
            };

            class ServiceException : public std::exception {
                private:
                std::string message;
                ExceptionType errorCode;

                public:
                ServiceException () : message{ "Unknown exception" }, errorCode{ ExceptionType::UNKNOWN_EXCEPTION } {
                }
                virtual ~ServiceException () {
                }
                void operator= (const ServiceException& other);
                const char* what () const noexcept;

                // Setters
                void setMessage (const std::string& message);
                void setErrorCode (const ExceptionType errorCode);

                // Getters
                std::string getMessage () const;
                ExceptionType getErrorCode () const;
            };

            class ClientException : public std::exception {
                private:
                std::string message;
                ExceptionType errorCode;

                public:
                ClientException () : message{ "Unknown exception" }, errorCode{ ExceptionType::UNKNOWN_EXCEPTION } {
                }
                virtual ~ClientException () {
                }
                void operator= (const ClientException& other);
                const char* what () const noexcept;

                // Setters
                void setMessage (const std::string& message);
                void setErrorCode (const ExceptionType errorCode);

                // Getters
                std::string getMessage () const;
                ExceptionType getErrorCode () const;
            };
        }
    }
}

#endif
