aux_source_directory(. lib_utils_srcs)
add_library(utils ${lib_utils_srcs})
target_include_directories(utils PUBLIC .)
