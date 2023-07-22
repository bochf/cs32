cmake_minimum_required(VERSION 3.22.0)
project(recursion VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(libs)

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(test)