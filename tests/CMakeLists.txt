add_executable(tests
    test_board.cpp
    test_agents.cpp
)

target_link_libraries(tests PRIVATE
    core
    agents
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)