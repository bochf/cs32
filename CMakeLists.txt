cmake_minimum_required(VERSION 3.0.0)
project(CS32 VERSION 0.1.0 LANGUAGES C CXX)

# include(CTest)
enable_testing()
find_package(GTest REQUIRED)

# add_library(sort sort.cpp)
# 
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

add_subdirectory(src)
add_subdirectory(test)
