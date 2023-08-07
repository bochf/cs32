aux_source_directory(. test_sort_srcs)
add_executable(test_sort ${test_sort_srcs})
target_link_libraries(test_sort sort)