#include <vector>

#include <benchmark/benchmark.h>

#include "containers/vector2.hpp"
#include "containers/pmr_allocator.hpp"
#include "containers/buffer_pmr.hpp"

#include "compiler_pragmas.hpp"

template <typename T>
using vec_pmr = ml::vector2<T, ml::pmr_allocator<T>>;
template <typename T, std::size_t CAPACITY>
using stack_pmr = ml::buffer_pmr<T, CAPACITY, ml::pmr>;

static constexpr int n_placements{400};
static constexpr int n_stack_capacity{1000};

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
        stack_pmr<int, n_stack_capacity> resource;
        vec_pmr<int> vec{&resource};

        for (int i = 0; i < n_placements; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vector2_std_iter(benchmark::State& state) {
    std::vector<int> vec;
    for (int i = 0; i < n_placements; ++i) {
        vec.emplace_back(i);
    }

    for (auto _ : state) {
        for (auto const& elem : vec) {
            benchmark::DoNotOptimize(elem);
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vector2_v2_stack_iter(benchmark::State& state) {
    stack_pmr<int, n_stack_capacity> resource;
    vec_pmr<int> vec{&resource};
    for (int i = 0; i < n_placements; ++i) {
        vec.emplace_back(i);
    }

    for (auto _ : state) {
        for (auto const& elem : vec) {
            benchmark::DoNotOptimize(elem);
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

//BENCHMARK(BM_vector2_std);
//BENCHMARK(BM_vector2_v2_stack);

//BENCHMARK(BM_vector2_std_iter);
//BENCHMARK(BM_vector2_v2_stack_iter);
