#include <list>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/multi_arena_resource.hpp"

#include "compiler_pragmas.hpp"

constexpr int n_placements{25'000};
constexpr int n_reserve{100};
constexpr int arena_elems_to_allocate{10'000};

using T = double;

static constexpr auto INITIAL_SIZE{sizeof(T) * arena_elems_to_allocate};

static void BM_vl_vector_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::vector<T>> vec{};
        for (int i = 0; i < n_placements; ++i) {
            vec.emplace_back();
            auto& inner_vec{vec.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_vec.push_back(static_cast<T>(i));
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vl_list_std(benchmark::State& state) {
    for (auto _ : state) {
        std::list<std::list<T>> list{};

        for (int i = 0; i < n_placements; ++i) {
            list.emplace_back();
            auto& inner_list{list.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_list.push_back(static_cast<T>(i));
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vl_vector_alloc(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
        std::pmr::vector<std::pmr::vector<T>> vec{&resource};

        for (int i = 0; i < n_placements; ++i) {
            vec.emplace_back();
            auto& inner_vec{vec.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_vec.push_back(static_cast<T>(i));
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vl_list_alloc(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
        std::pmr::list<std::pmr::list<T>> list{&resource};

        for (int i = 0; i < n_placements; ++i) {
            list.emplace_back();
            auto& inner_list{list.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_list.push_back(static_cast<T>(i));
            }
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

BENCHMARK(BM_vl_vector_std);
BENCHMARK(BM_vl_vector_alloc);
BENCHMARK(BM_vl_list_std);
BENCHMARK(BM_vl_list_alloc);
