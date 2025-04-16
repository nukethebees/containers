#pragma once

#include <cstddef>
#include <vector>

#include "arena_memory_resource.hpp"
#include "misc.hpp"

namespace ml {
class multi_arena_memory_resource_pmr {
  private:
    std::vector<ArenaMemoryResourcePmr> resources_;
  public:
    multi_arena_memory_resource_pmr() = delete;
    multi_arena_memory_resource_pmr(std::size_t n_resources, std::size_t initial_capacity);

    auto get_resource(std::size_t i) -> ArenaMemoryResourcePmr*;
};

template <typename... Types>
class multi_t_arena_memory_resource_pmr {
  private:
    static inline constexpr auto indexer{types_to_index<Types...>{}};
    std::vector<ArenaMemoryResourcePmr> resources_;
  public:
    multi_t_arena_memory_resource_pmr(std::size_t initial_capacity) {
        resources_.reserve(indexer.size());

        for (std::size_t i = 0; i < indexer.size(); ++i) {
            resources_.emplace_back(initial_capacity);
        }
    }
    template <typename T>
    auto get_resource() -> ArenaMemoryResourcePmr* {
        return &resources_[indexer.get<T>()];
    }
};
}
