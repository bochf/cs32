aux_source_directory(. test_cricketer_srcs)
add_executable(test_cricketer ${test_cricketer_srcs})
target_link_libraries(test_cricketer cricketer)
target_link_libraries(test_cricketer GTest::gtest_main)