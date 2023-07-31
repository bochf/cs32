add_executable(test_cricketer test_cricketer.cpp)
target_link_libraries(test_cricketer cricketer)
target_link_libraries(test_cricketer GTest::gtest_main)
add_test(NAME test_cricketer COMMAND $<TARGET_FILE : test_cricketer>)
