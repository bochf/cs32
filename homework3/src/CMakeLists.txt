aux_source_directory(. exe_sort_srcs)
add_executable(homework3 ${exe_sort_srcs})
target_link_libraries(homework3 sort)