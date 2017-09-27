cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
project(AuthLibrary)
enable_testing()

# Paths
set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/headers)

# Subprojects
add_subdirectory(src)
add_subdirectory(headers)
add_subdirectory(lib)
add_subdirectory(tst)



