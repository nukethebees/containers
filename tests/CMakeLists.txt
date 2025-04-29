enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(tests)
add_executable(containers::tests ALIAS tests)
target_sources(tests PRIVATE
    test_span.cpp
    test_array.cpp
    test_vector.cpp
    test_arena_allocator.cpp
    test_stack_buffer_allocator.cpp
    "test_multi_arena_resource.cpp" 
    "test_misc.cpp"
    "test_polymorphic_allocator.cpp"
 "test_slist.cpp")
target_compile_options(tests PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/FAs /Fa${CMAKE_BINARY_DIR}/asm\\>
)
target_compile_options(tests PRIVATE 
    /Zi
)
target_link_options(tests PRIVATE 
    /DEBUG
)

target_link_libraries(tests PRIVATE 
    GTest::gtest_main 
    containers::containers
)
target_include_directories(tests PRIVATE
    containers::containers
)