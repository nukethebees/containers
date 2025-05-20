#include <list>
#include <vector>

#include <benchmark/benchmark.h>

#include "containers/arena_mmr.hpp"
#include "containers/multi_arena_resource.hpp"

#include "compiler_pragmas.hpp"

constexpr int n_placements{25'000};
constexpr int n_reserve{100};
constexpr int arena_elems_to_allocate{1'000};

constexpr int ITER_ELEMS{n_placements * n_reserve};
constexpr int N_READ_LOOPS{1};

struct TestStruct {
    int a;
    double b;
    std::string c;

    TestStruct(int i)
        : a(i)
        , b(static_cast<int>(i))
        , c("This is a very long string that will need dynamic allocation.") {}
};
struct TestStructPmr {
    int a;
    double b;
    std::pmr::string c;

    TestStructPmr(int i, std::pmr::memory_resource* resource)
        : a(i)
        , b(static_cast<int>(i))
        , c("This is a very long string that will need dynamic allocation.", resource) {}
};

using TA = TestStruct;
using TP = TestStructPmr;

static constexpr auto INITIAL_SIZE{sizeof(TP) * arena_elems_to_allocate};

// #define ITERATE_AFTER

#define READ_LOOP                                       \
    for (int i = 0; i < N_READ_LOOPS; i++) {            \
        for (auto const& inner_container : container) { \
            for (auto const& item : inner_container) {  \
                benchmark::DoNotOptimize(item);         \
            }                                           \
        }                                               \
    }

static void BM_vl_wr_vector_std(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<std::vector<TA>> container{};
        for (int i = 0; i < n_placements; ++i) {
            container.emplace_back();
            auto& inner_container{container.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_container.emplace_back(i);
            }
        }

#ifdef ITERATE_AFTER
        READ_LOOP
#endif
    }
    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_wr_list_std(benchmark::State& state) {
    for (auto _ : state) {
        std::list<std::list<TA>> container{};

        for (int i = 0; i < n_placements; ++i) {
            container.emplace_back();
            auto& inner_container{container.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_container.emplace_back(i);
            }
        }

#ifdef ITERATE_AFTER
        READ_LOOP
#endif
    }
    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_wr_vector_alloc(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
        std::pmr::vector<std::pmr::vector<TP>> container{&resource};

        for (int i = 0; i < n_placements; ++i) {
            container.emplace_back();
            auto& inner_container{container.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_container.emplace_back(i, &resource);
            }
        }

#ifdef ITERATE_AFTER
        READ_LOOP
#endif
    }
    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_wr_list_alloc(benchmark::State& state) {
    for (auto _ : state) {
        ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
        std::pmr::list<std::pmr::list<TP>> container{&resource};

        for (int i = 0; i < n_placements; ++i) {
            container.emplace_back();
            auto& inner_container{container.back()};

            for (int j = 0; j < n_reserve; j++) {
                inner_container.emplace_back(i, &resource);
            }
        }

#ifdef ITERATE_AFTER
        READ_LOOP
#endif
    }
    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}

static void BM_vl_rd_vector_std(benchmark::State& state) {
    std::vector<std::vector<TA>> container{};
    for (int i = 0; i < n_placements; ++i) {
        container.emplace_back();
        auto& inner_container{container.back()};

        for (int j = 0; j < n_reserve; j++) {
            inner_container.emplace_back(i);
        }
    }

    for (auto _ : state) {
        READ_LOOP
    }

    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_rd_list_std(benchmark::State& state) {
    std::list<std::list<TA>> container{};

    for (int i = 0; i < n_placements; ++i) {
        container.emplace_back();
        auto& inner_container{container.back()};

        for (int j = 0; j < n_reserve; j++) {
            inner_container.emplace_back(i);
        }
    }

    for (auto _ : state) {
        READ_LOOP
    }

    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_rd_vector_alloc(benchmark::State& state) {
    ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
    std::pmr::vector<std::pmr::vector<TP>> container{&resource};

    for (int i = 0; i < n_placements; ++i) {
        container.emplace_back();
        auto& inner_container{container.back()};

        for (int j = 0; j < n_reserve; j++) {
            inner_container.emplace_back(i, &resource);
        }
    }

    for (auto _ : state) {
        READ_LOOP
    }

    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}
static void BM_vl_rd_list_alloc(benchmark::State& state) {
    ml::ArenaMemoryResourcePmr resource{INITIAL_SIZE};
    std::pmr::list<std::pmr::list<TP>> container{&resource};

    for (int i = 0; i < n_placements; ++i) {
        container.emplace_back();
        auto& inner_container{container.back()};

        for (int j = 0; j < n_reserve; j++) {
            inner_container.emplace_back(i, &resource);
        }
    }

    for (auto _ : state) {
        READ_LOOP
    }

    state.SetItemsProcessed(state.iterations() * ITER_ELEMS);
}

//BENCHMARK(BM_vl_wr_vector_std);
//BENCHMARK(BM_vl_wr_vector_alloc);
//BENCHMARK(BM_vl_wr_list_std);
//BENCHMARK(BM_vl_wr_list_alloc);
//
//BENCHMARK(BM_vl_rd_vector_std);
//BENCHMARK(BM_vl_rd_vector_alloc);
//BENCHMARK(BM_vl_rd_list_std);
//BENCHMARK(BM_vl_rd_list_alloc);
//