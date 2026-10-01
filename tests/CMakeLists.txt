add_executable(tests
    test_board.cpp
)

target_link_libraries(tests PRIVATE
    core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)