find_package(benchmark REQUIRED)

add_executable (benchmarks)
add_executable (containers::benchmarks ALIAS benchmarks)

target_sources(benchmarks PRIVATE
  "main.cpp"
  "bm_vector.cpp" 
  "bm_vector_stackbuf.cpp" 
  "bm_mixed_vector.cpp" 
  "bm_vec_list.cpp" 
  "bm_vector2.cpp" 
  "bm_sorting.cpp"
  "bm_stack_pmr.cpp"
)

target_link_libraries(benchmarks PRIVATE
  benchmark::benchmark 
  containers::containers
)


if (MSVC)
  target_compile_options(benchmarks PRIVATE /Zi /FAs /Fa${CMAKE_BINARY_DIR}/asm\\)
  target_link_options(benchmarks PUBLIC /PROFILE /DEBUG)
endif()