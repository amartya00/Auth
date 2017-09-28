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

