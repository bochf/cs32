aux_source_directory(. test_recursion_src)
add_executable(test_recursion ${test_recursion_src})
target_link_libraries(test_recursion GTest::gtest_main)
target_link_libraries(test_recursion recursion)