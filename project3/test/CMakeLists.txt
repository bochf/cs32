add_executable(test_anagrams test_anagrams.cpp)
target_link_libraries(test_anagrams anagrams)
target_link_libraries(test_anagrams GTest::gtest_main)
add_test(NAME test_anagrams COMMAND $<TARGET_FILE : test_anagrams>)
