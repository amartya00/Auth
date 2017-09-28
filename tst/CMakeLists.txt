cmake_minimum_required(VERSION 3.0)
include(ExternalProject)
find_package(Git REQUIRED)

# Build mock library
add_library(authmocks SHARED
    "mocks/MockDynamoDBClient.cc"
    "mocks/MockGapiWrapper.cc"
    )

target_link_libraries(
    authmocks
    aws-cpp-sdk-core
    aws-cpp-sdk-dynamodb)

# Add catch as external project.
ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 100
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

ExternalProject_Get_Property(catch source_dir)
set(CATCH_HEADERS ${source_dir}/single_include)

include_directories(${HEADERS})
include_directories(${CATCH_HEADERS})

add_executable(UserDataTest "datastructures/UserItemTest.cc")
add_dependencies(UserDataTest catch)
target_link_libraries(UserDataTest auth)

add_executable(WhiteListedUserTest "datastructures/WhiteListedUserTest.cc")
add_dependencies(WhiteListedUserTest catch)
target_link_libraries(WhiteListedUserTest auth)

add_executable(WhitelistEnabledServiceTest "datastructures/WhitelistEnabledServiceTest.cc")
add_dependencies(WhitelistEnabledServiceTest catch)
target_link_libraries(WhitelistEnabledServiceTest auth)

add_executable(SigninTest "auth/SigninTest.cc")
add_dependencies(SigninTest catch)
target_link_libraries(SigninTest auth authmocks)

add_test(UserDataTest UserDataTest)
add_test(WhiteListedUserTest WhiteListedUserTest)
add_test(WhitelistEnabledServiceTest WhitelistEnabledServiceTest)
add_test(SigninTest SigninTest)
