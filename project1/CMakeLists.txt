cmake_minimum_required(VERSION 3.22.0)
project(zion VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(libs)
add_subdirectory(src)

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(test)
