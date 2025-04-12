#include <cstddef>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/stack_buffer_memory_resource.hpp"

template <typename T>
using pool_vec = std::vector<T, ml::ArenaAllocator<T>>;

constexpr int n_placements{2500};
constexpr int n_reserve{10};
constexpr int n_repetitions{5};

static void BM_vector_alloc_std(benchmark::State & state) {
    for (auto _ : state) {
        std::vector<std::vector<double>> d_vecs;
        std::vector<std::vector<char>> c_vecs;

        for (int i = 0; i < n_placements; ++i) {
            std::vector<double> double_vec;
            double_vec.reserve(n_reserve);
            d_vecs.emplace_back(std::move(double_vec));

            std::vector<char> char_vec;
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }

        benchmark::DoNotOptimize(1);
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_vector_alloc_arena(benchmark::State & state) {
    constexpr int elems_to_allocate{10'000};

    for (auto _ : state) {
        ml::ArenaMemoryResource char_resource{sizeof(char) * elems_to_allocate};
        ml::ArenaMemoryResource double_resource{sizeof(double) * elems_to_allocate};

        ml::ArenaAllocator<char> c_alloc{&char_resource};
        ml::ArenaAllocator<double> d_alloc{&double_resource};

        std::vector<pool_vec<char>> c_vecs;
        std::vector<pool_vec<double>> d_vecs;

        for (int i = 0; i < n_placements; ++i) {
            pool_vec<double> double_vec{d_alloc};
            double_vec.reserve(n_reserve);
            d_vecs.emplace_back(std::move(double_vec));

            pool_vec<char> char_vec{c_alloc};
            char_vec.reserve(n_reserve);
            c_vecs.emplace_back(std::move(char_vec));
        }

        benchmark::DoNotOptimize(1);
    }
    state.SetItemsProcessed(state.iterations());
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
//BENCHMARK(BM_vector_alloc_arena)->Repetitions(n_repetitions);

BENCHMARK(BM_vec_stackbuf_new_ref)->Repetitions(n_repetitions);
BENCHMARK(BM_vec_stackbuf)->Repetitions(n_repetitions);
BENCHMARK_MAIN();
