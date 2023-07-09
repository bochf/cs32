add_executable(ori_zion zionSolution.cpp)

add_executable(test_previous test_previous.cpp)
target_link_libraries(test_previous zion)
add_test(NAME test_previous COMMAND $<TARGET_FILE : test_previous>)

add_executable(test_all_headers test_all_headers.cpp)
target_link_libraries(test_all_headers zion)
add_test(NAME test_all_headers COMMAND $<TARGET_FILE : test_all_headers>)

add_executable(test_robot test_robot.cpp)
target_link_libraries(test_robot zion)
target_link_libraries(test_robot GTest::gtest_main)
add_test(NAME test_robot COMMAND $<TARGET_FILE : test_robot>)

add_executable(test_player test_player.cpp)
target_link_libraries(test_player zion)
target_link_libraries(test_player GTest::gtest_main)
add_test(NAME test_player COMMAND $<TARGET_FILE : test_player>)

add_executable(test_arena test_arena.cpp)
target_link_libraries(test_arena zion)
target_link_libraries(test_arena GTest:: gtest_main)
add_test(NAME test_arena COMMAND $<TARGET_FILE : test_arena>)

add_executable(test_arena_player1 test_arena_player1 .cpp)
target_link_libraries(test_arena_player1 zion)
target_link_libraries(test_arena_player1 GTest::gtest_main)
add_test(NAME test_arena_player1 COMMAND $<TARGET_FILE : test_arena_player1>)

add_executable(test_arena_player2 test_arena_player2 .cpp)
target_link_libraries(test_arena_player2 zion)
target_link_libraries(test_arena_player2 GTest::gtest_main)
add_test(NAME test_arena_player2 COMMAND $<TARGET_FILE : test_arena_player2>)

add_executable(test_arena_player3 test_arena_player3.cpp)
target_link_libraries(test_arena_player3 zion)
target_link_libraries(test_arena_player3 GTest::gtest_main)
add_test(NAME test_arena_player3 COMMAND $<TARGET_FILE : test_arena_player3>)

add_executable(test_show test_show .cpp)
target_link_libraries(test_show zion)
target_link_libraries(test_show GTest:: gtest_main)
add_test(NAME test_show COMMAND $<TARGET_FILE : test_show>)
