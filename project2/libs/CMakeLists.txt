aux_source_directory(. lib_cricketer_srcs)
add_library(cricketer ${lib_cricketer_srcs})
target_include_directories(cricketer PUBLIC .)