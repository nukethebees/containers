add_executable(tests test_span.cpp)

include_directories(../)

target_link_libraries(tests PRIVATE GTest::gtest_main Containers)
gtest_discover_tests(tests)
