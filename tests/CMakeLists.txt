enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(tests)
add_executable(containers::tests ALIAS tests)
target_sources(tests PRIVATE
  "test_array.cpp"
  "test_arena_allocator.cpp"
  "test_binary_heap.cpp"
  "test_bst.cpp" 
  "test_buffer_memory_resource.cpp"
  "test_dlist.cpp"
  "test_linked_vector.cpp" 
  "test_misc.cpp"
  "test_multi_arena_resource.cpp" 
  "test_polymorphic_allocator.cpp"
  "test_rbset.cpp" 
  "test_slist.cpp"
  "test_sort.cpp"
  "test_span.cpp"
  "test_static_vector.cpp"  
  "test_vector.cpp"
  "test_vector2.cpp"
)

target_sources(tests PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ../
  FILES
  "test_container_single_elem_iterators.hpp"
  "test_sorting.hpp"
)

if (MSVC)
  target_compile_options(tests PRIVATE 
    /FAs
    /Fa${CMAKE_BINARY_DIR}/asm\\
  )
  target_compile_options(tests PRIVATE /Zi)
  target_link_options(tests PRIVATE /DEBUG)
endif()

target_link_libraries(tests PRIVATE 
    GTest::gtest_main 
    containers::containers
)
target_include_directories(tests PRIVATE
    containers::containers
)