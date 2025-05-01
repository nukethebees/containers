find_package(benchmark REQUIRED)

add_executable (benchmarks)
add_executable (containers::benchmarks ALIAS benchmarks)

target_sources(benchmarks PRIVATE
  "main.cpp"
)

target_link_libraries(benchmarks PRIVATE
  benchmark::benchmark 
  containers::containers
)


if (MSVC)
  target_compile_options(benchmarks PRIVATE /Zi /FAs /Fa${CMAKE_BINARY_DIR}/asm\\)
  target_link_options(benchmarks PUBLIC /PROFILE /DEBUG)
endif()