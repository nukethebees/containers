find_package(benchmark REQUIRED)

include_directories("../")
add_executable (benchmarks "main.cpp")
target_link_libraries(benchmarks benchmark::benchmark Containers)

if (MSVC)
    target_link_options(benchmarks PRIVATE /DEBUG)
endif()