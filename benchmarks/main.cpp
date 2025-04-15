#include <cstddef>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/stack_buffer_memory_resource.hpp"

constexpr int n_placements{25000};
constexpr int n_reserve{10};
constexpr int n_repetitions{5};

constexpr int arena_elems_to_allocate{10'000};

static void BM_vector_alloc_std(benchmark::State & state) {
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
static void BM_vector_alloc_arena(benchmark::State & state) {
    for (auto _ : state) {
        ml::ArenaMemoryResource char_resource{sizeof(char) * arena_elems_to_allocate};
        ml::ArenaAllocator<char> c_alloc{&char_resource};
        std::vector<std::vector<char, ml::ArenaAllocator<char>>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::vector<char, ml::ArenaAllocator<char>> char_vec{c_alloc};
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

static void BM_vector_alloc_arena_pmr(benchmark::State & state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr char_resource{sizeof(char) * arena_elems_to_allocate};
        std::vector<std::pmr::vector<char>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::pmr::vector<char> char_vec{&char_resource};
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements);
}

// Comparison benchmark vs the buffer version
static void BM_vec_stackbuf_new_ref(benchmark::State & state) {
    static constexpr std::size_t n_ints{50};

    for (auto _ : state) {
        std::vector<int> vec{};
        vec.reserve(n_ints);
        for (int i{0}; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_vec_stackbuf(benchmark::State & state) {
    static constexpr std::size_t n_ints{50};
    using config = ml::StackAllocConfig<int, n_ints>;

    for (auto _ : state) {
        config::Resource resource;
        config::Allocator alloc{&resource};
        std::vector<int, config::Allocator> vec{alloc};
        vec.reserve(n_ints);
        for (int i{0}; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

//BENCHMARK(BM_vector_alloc_std)->Repetitions(n_repetitions);

BENCHMARK(BM_vector_alloc_arena);
BENCHMARK(BM_vector_alloc_arena_pmr);

//BENCHMARK(BM_vec_stackbuf_new_ref)->Repetitions(n_repetitions);
//BENCHMARK(BM_vec_stackbuf)->Repetitions(n_repetitions);
BENCHMARK_MAIN();
