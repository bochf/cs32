cmake_minimum_required(VERSION 3.0.0)
project(homework1 VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(libs)
add_subdirectory(src)

find_package(GTest REQUIRED)
include(CTest)
enable_testing()
add_subdirectory(test)