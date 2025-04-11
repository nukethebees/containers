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
    target_link_options(benchmarks PRIVATE /DEBUG)
endif()