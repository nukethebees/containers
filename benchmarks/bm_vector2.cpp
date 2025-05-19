#include <vector>

#include <benchmark/benchmark.h>

#include "containers/vector2.hpp"
#include "containers/polymorphic_allocator.hpp"
#include "containers/stack_buffer_memory_resource_pmr.hpp"

#include "compiler_pragmas.hpp"

template <typename T>
using vec_pmr = ml::vector2<T, ml::polymorphic_allocator<T>>;
template <typename T, std::size_t CAPACITY>
using stack_pmr = ml::stack_buffer_pmr<T, CAPACITY, ml::memory_resource>;

static constexpr int n_placements{400};

static void BM_vector2_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<int> vec;

        for (int i = 0; i < n_placements; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vector2_v2_stack(benchmark::State& state) {
    for (auto _ : state) {
        stack_pmr<int, 1000> resource;
        vec_pmr<int> vec{&resource};

        for (int i = 0; i < n_placements; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

BENCHMARK(BM_vector2_std);
BENCHMARK(BM_vector2_v2_stack);
