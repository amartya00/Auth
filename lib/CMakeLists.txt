cmake_minimum_required(VERSION 3.0)
add_compile_options(-fPIC)

include_directories(${HEADERS})

add_library(gapiwrapper SHARED
    "gapi/GapiWrapper.cc")

add_library(auth SHARED
    "datastructures/UserItem.cc"
    "datastructures/WhiteListedUser.cc"
    "datastructures/WhitelistEnabledService.cc"

    "utils/AwsStringTranslations.cc"

    "exceptions/Exceptions.cc"

    "auth/Service.cc"
    "auth/SigninOrRegister.cc"
    "auth/IsWhitelistingEnabledForService.cc"
    "auth/IsUserWhitelisted.cc"
    "auth/IsUserRegisteredForService.cc"
    "auth/WriteUserDate.cc"
    "auth/AuthorizeUser.cc"
    )

target_link_libraries(
    gapiwrapper
    curl)

target_link_libraries(
    auth
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb
    jsoncpp
    curl
    gapiwrapper)

