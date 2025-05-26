#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/bubble_sort.hpp"
#include "containers/heap_sort.hpp"
#include "containers/insertion_sort.hpp"
#include "containers/merge_sort.hpp"
#include "containers/quick_sort.hpp"
#include "containers/radix_sort.hpp"
#include "containers/selection_sort.hpp"

#include "compiler_pragmas.hpp"

static constexpr std::size_t n_elems{25000};

template <typename NumT = int>
static auto create_elems() {
    static std::vector<NumT> ints;
    static bool initialised{false};

    if (!initialised) {
        initialised = true;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<NumT> distr(std::numeric_limits<NumT>::min(), std::numeric_limits<NumT>::max());

        for (std::size_t i{0}; i < n_elems; i++) {
            ints.push_back(distr(gen));
        }
    }

    return ints;
}

static void BM_sorting_std(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        std::sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_bubble(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::bubble_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_heap(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::heap_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_insertion(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::insertion_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_merge(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::merge_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_quick(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::quick_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_radix(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems<unsigned int>()};
        ml::radix_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_sorting_selection(benchmark::State& state) {
    for (auto _ : state) {
        auto nums{create_elems()};
        ml::selection_sort(nums.begin(), nums.end());
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_sorting_std);
BENCHMARK(BM_sorting_bubble);
BENCHMARK(BM_sorting_heap);
BENCHMARK(BM_sorting_insertion);
BENCHMARK(BM_sorting_merge);
BENCHMARK(BM_sorting_quick);
BENCHMARK(BM_sorting_radix);
BENCHMARK(BM_sorting_selection);
