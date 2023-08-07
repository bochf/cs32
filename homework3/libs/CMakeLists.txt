aux_source_directory(. lib_sort_srcs)
add_library(sort ${lib_sort_srcs})
target_include_directories(sort PUBLIC .)