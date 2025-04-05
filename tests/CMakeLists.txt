add_executable(tests test_span.cpp "test_array.cpp")
target_compile_options(tests PRIVATE /FAs /Fa${CMAKE_BINARY_DIR}/asm\\)

include_directories(../)

target_link_libraries(tests PRIVATE GTest::gtest_main Containers)
gtest_discover_tests(tests)
