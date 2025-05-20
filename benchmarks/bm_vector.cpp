#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_allocator.hpp"
#include "containers/arena_mmr.hpp"
#include "containers/arena_memory_resource_pmr.hpp"

#include "compiler_pragmas.hpp"

constexpr int n_placements{ 25000 };
constexpr int n_reserve{ 10 };

constexpr int arena_elems_to_allocate{ 10'000 };

static void BM_vector_alloc_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::vector<char>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::vector<char> char_vec;
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}
static void BM_vector_alloc_arena(benchmark::State& state) {
    for (auto _ : state) {
        ml::arena_mmr char_resource{ sizeof(char) * arena_elems_to_allocate };
        ml::ArenaAllocator<char> c_alloc{ &char_resource };
        std::vector<std::vector<char, ml::ArenaAllocator<char>>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::vector<char, ml::ArenaAllocator<char>> char_vec{ c_alloc };
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

static void BM_vector_alloc_arena_pmr(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr char_resource{ sizeof(char) * arena_elems_to_allocate };
        std::vector<std::pmr::vector<char>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::pmr::vector<char> char_vec{ &char_resource };
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}


