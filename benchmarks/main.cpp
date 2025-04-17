#include <cstddef>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/stack_buffer_memory_resource.hpp"
#include "containers/multi_arena_resource.hpp"
#include "containers/misc.hpp"

constexpr int n_placements{25000};
constexpr int n_reserve{10};
constexpr int n_repetitions{5};

constexpr int arena_elems_to_allocate{10'000};

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

static void BM_vector_alloc_arena_pmr(benchmark::State& state) {
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
static void BM_vec_stackbuf_new_ref(benchmark::State& state) {
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
static void BM_vec_stackbuf(benchmark::State& state) {
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

static void BM_mixed_vector_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<char> char_vec{};
        std::vector<double> double_vec{};
        std::vector<std::string> string_vec{};

        for (int i = 0; i < n_placements; ++i) {
            char_vec.push_back(static_cast<char>(i % 256));
            double_vec.push_back(i * 1.1);
            string_vec.emplace_back(50, 'a' + (i % 26));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements * 3);
}
static void BM_mixed_vector_single_resource(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr resource{sizeof(char) * arena_elems_to_allocate};
        std::pmr::polymorphic_allocator<char> char_alloc{&resource};
        std::pmr::polymorphic_allocator<double> double_alloc{&resource};
        std::pmr::polymorphic_allocator<std::string> string_alloc{&resource};

        std::pmr::vector<char> char_vec{char_alloc};
        std::pmr::vector<double> double_vec{double_alloc};
        std::pmr::vector<std::string> string_vec{string_alloc};

        for (int i = 0; i < n_placements; ++i) {
            char_vec.push_back(static_cast<char>(i % 256));
            double_vec.push_back(i * 1.1);
            string_vec.emplace_back(50, 'a' + (i % 26));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements * 3);
}
static void BM_mixed_vector_multipool_allocator(benchmark::State& state) {
    for (auto _ : state) {
        constexpr std::size_t n_resources = 3;

        ml::MultiArenaMemoryResourcePmr resource{n_resources, sizeof(char) * arena_elems_to_allocate};

        auto* char_arena = resource.get_resource(0);
        auto* double_arena = resource.get_resource(1);
        auto* string_arena = resource.get_resource(2);

        std::pmr::polymorphic_allocator<char> char_alloc{char_arena};
        std::pmr::polymorphic_allocator<double> double_alloc{double_arena};
        std::pmr::polymorphic_allocator<std::string> string_alloc{string_arena};

        std::pmr::vector<char> char_vec{char_alloc};
        std::pmr::vector<double> double_vec{double_alloc};
        std::pmr::vector<std::pmr::string> string_vec{string_alloc};

        for (int i = 0; i < n_placements; ++i) {
            char_vec.push_back(static_cast<char>(i % 256));
            double_vec.push_back(i * 1.1);
            string_vec.emplace_back(50, 'a' + (i % 26));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements * 3);
}
static void BM_mixed_vector_multipool_t_allocator(benchmark::State& state) {
    for (auto _ : state) {
        ml::MultiTArenaMemoryResourcePmr<char, double, std::pmr::string> resource{sizeof(char) *
                                                                                  arena_elems_to_allocate};

        auto* char_arena = resource.get_resource<char>();
        auto* double_arena = resource.get_resource<double>();
        auto* string_arena = resource.get_resource<std::pmr::string>();

        std::pmr::polymorphic_allocator<char> char_alloc{char_arena};
        std::pmr::polymorphic_allocator<double> double_alloc{double_arena};
        std::pmr::polymorphic_allocator<std::string> string_alloc{string_arena};

        std::pmr::vector<char> char_vec{char_alloc};
        std::pmr::vector<double> double_vec{double_alloc};
        std::pmr::vector<std::pmr::string> string_vec{string_alloc};

        for (int i = 0; i < n_placements; ++i) {
            char_vec.push_back(static_cast<char>(i % 256));
            double_vec.push_back(i * 1.1);
            string_vec.emplace_back(50, 'a' + (i % 26));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements * 3);
}

BENCHMARK(BM_mixed_vector_std);
BENCHMARK(BM_mixed_vector_single_resource);
BENCHMARK(BM_mixed_vector_multipool_allocator);
BENCHMARK(BM_mixed_vector_multipool_t_allocator);


BENCHMARK_MAIN();
