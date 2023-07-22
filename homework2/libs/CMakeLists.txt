aux_source_directory(. lib_recursion_src)
add_library(recursion ${lib_recursion_src})
target_include_directories(recursion PUBLIC .)