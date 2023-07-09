aux_source_directory(. test_linked_list_src)
add_executable(test_linked_list ${test_linked_list_src})
target_link_libraries(test_linked_list linked_list)
target_link_libraries(test_linked_list GTest::gtest_main)
add_test(
  NAME test_linked_list 
  COMMAND $<TARGET_FILE: test_linked_list>)