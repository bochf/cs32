aux_source_directory(. lib_linked_list_src)
add_library(linked_list ${lib_linked_list_src})
target_include_directories(linked_list PUBLIC .)