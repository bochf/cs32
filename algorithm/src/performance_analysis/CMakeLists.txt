aux_source_directory(. lib_perf_srcs)
add_library(perf ${lib_perf_srcs})
target_include_directories(perf PUBLIC .)
