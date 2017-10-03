cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
project(AuthLibrary
    VERSION 0.1
    LANGUAGES CXX)
enable_testing()

# Paths
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/headers)
set(API_VERSION 0)

# Subprojects
add_subdirectory(src)
add_subdirectory(headers)
add_subdirectory(lib)
add_subdirectory(tst)



