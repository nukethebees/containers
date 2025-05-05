#include <vector>

#include <benchmark/benchmark.h>

#include "containers/stack_buffer_memory_resource.hpp"

#include "compiler_pragmas.hpp"

constexpr int n_placements{ 25000 };
constexpr int n_reserve{ 10 };

constexpr int arena_elems_to_allocate{ 10'000 };

// Comparison benchmark vs the buffer version
static void BM_vec_stackbuf_new_ref(benchmark::State& state) {
    static constexpr std::size_t n_ints{ 50 };

    for (auto _ : state) {
        std::vector<int> vec{};
        vec.reserve(n_ints);
        for (std::size_t i{ 0 }; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_vec_stackbuf(benchmark::State& state) {
    static constexpr std::size_t n_ints{ 50 };
    using config = ml::StackAllocConfig<int, n_ints>;

    for (auto _ : state) {
        config::Resource resource;
        config::Allocator alloc{ &resource };
        std::vector<int, config::Allocator> vec{ alloc };
        vec.reserve(n_ints);
        for (std::size_t i{ 0 }; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

