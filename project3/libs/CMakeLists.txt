aux_source_directory(. lib_anagrams_srcs)
add_library(anagrams ${lib_anagrams_srcs})
target_include_directories(anagrams PUBLIC .)
