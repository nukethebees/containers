#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/multi_arena_resource.hpp"

#include "compiler_pragmas.hpp"

constexpr int n_placements{ 25000 };
constexpr int n_reserve{ 10 };

constexpr int arena_elems_to_allocate{ 10'000 };

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
        ml::ArenaMemoryResourcePmr resource{ sizeof(char) * arena_elems_to_allocate };
        std::pmr::polymorphic_allocator<char> char_alloc{ &resource };
        std::pmr::polymorphic_allocator<double> double_alloc{ &resource };
        std::pmr::polymorphic_allocator<std::string> string_alloc{ &resource };

        std::pmr::vector<char> char_vec{ char_alloc };
        std::pmr::vector<double> double_vec{ double_alloc };
        std::pmr::vector<std::string> string_vec{ string_alloc };

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

        ml::MultiArenaMemoryResourcePmr resource{ n_resources, sizeof(char) * arena_elems_to_allocate };

        auto* char_arena = resource.get_resource(0);
        auto* double_arena = resource.get_resource(1);
        auto* string_arena = resource.get_resource(2);

        std::pmr::polymorphic_allocator<char> char_alloc{ char_arena };
        std::pmr::polymorphic_allocator<double> double_alloc{ double_arena };
        std::pmr::polymorphic_allocator<std::string> string_alloc{ string_arena };

        std::pmr::vector<char> char_vec{ char_alloc };
        std::pmr::vector<double> double_vec{ double_alloc };
        std::pmr::vector<std::pmr::string> string_vec{ string_alloc };

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
        ml::MultiTArenaMemoryResourcePmr<char, double, std::pmr::string> resource{ sizeof(char) *
                                                                                  arena_elems_to_allocate };

        auto* char_arena = resource.get_resource<char>();
        auto* double_arena = resource.get_resource<double>();
        auto* string_arena = resource.get_resource<std::pmr::string>();

        std::pmr::polymorphic_allocator<char> char_alloc{ char_arena };
        std::pmr::polymorphic_allocator<double> double_alloc{ double_arena };
        std::pmr::polymorphic_allocator<std::string> string_alloc{ string_arena };

        std::pmr::vector<char> char_vec{ char_alloc };
        std::pmr::vector<double> double_vec{ double_alloc };
        std::pmr::vector<std::pmr::string> string_vec{ string_alloc };

        for (int i = 0; i < n_placements; ++i) {
            char_vec.push_back(static_cast<char>(i % 256));
            double_vec.push_back(i * 1.1);
            string_vec.emplace_back(50, 'a' + (i % 26));
        }
    }
    state.SetItemsProcessed(state.iterations() * n_placements * 3);
}

//BENCHMARK(BM_mixed_vector_std);
//BENCHMARK(BM_mixed_vector_single_resource);
//BENCHMARK(BM_mixed_vector_multipool_allocator);
//BENCHMARK(BM_mixed_vector_multipool_t_allocator);

