aux_source_directory(. test_sort_src)
add_executable(test_sort ${test_sort_src})
target_link_libraries(test_sort sort)
target_link_libraries(test_sort GTest::gtest_main)
