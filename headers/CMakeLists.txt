cmake_minimum_required(VERSION 3.0)

include_directories(${HEADERS})

add_custom_target(Headers
    SOURCES
    "DataStructures.h"
    "Auth.h"
    "Utils.h"
    "Exceptions.h"
    "Mocks.h"
    "GapiWrapper.h")


set(HEADER_FILES
    "DataStructures.h"
    "Auth.h"
    "Utils.h"
    "Exceptions.h"
    "Mocks.h"
    "GapiWrapper.h")

install(FILES ${HEADER_FILES} DESTINATION /usr/include/auth)
