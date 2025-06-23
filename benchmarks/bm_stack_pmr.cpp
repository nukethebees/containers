#include <vector>

#include <benchmark/benchmark.h>

#include "containers/stack_pmr.hpp"

#include "compiler_pragmas.hpp"

static constexpr int n_ints{50};

// Comparison benchmark vs the buffer version
static void BM_stack_pmr_intvec_new(benchmark::State& state) {
    for (auto _ : state) {
        std::pmr::vector<int> vec{};
        for (int i{0}; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}
static void BM_stack_pmr_intvec_stack(benchmark::State& state) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);

    for (auto _ : state) {
        auto frame{stack.create_frame()};
        std::pmr::vector<int> vec{frame.get()};
        for (int i{0}; i < n_ints; ++i) {
            vec.emplace_back(i);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_stack_pmr_intvec_new);
BENCHMARK(BM_stack_pmr_intvec_stack);
