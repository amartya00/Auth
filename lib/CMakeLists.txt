cmake_minimum_required(VERSION 3.0)
add_compile_options(-fPIC)

include_directories(${HEADERS})
add_library(auth SHARED
    "datastructures/UserItem.cc"
    "datastructures/WhiteListedUser.cc"
    "datastructures/WhitelistEnabledService.cc"

    "utils/AwsStringTranslations.cc"
    )

target_link_libraries(
    auth
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb)
