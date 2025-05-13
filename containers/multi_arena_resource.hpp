#pragma once

#include <cstddef>
#include <vector>

#include "arena_memory_resource.hpp"
#include "arena_memory_resource_pmr.hpp"
#include "misc.hpp"

namespace ml {
class MultiArenaMemoryResourcePmr {
  private:
    std::vector<ArenaMemoryResourcePmr> resources_;
  public:
    MultiArenaMemoryResourcePmr() = delete;
    MultiArenaMemoryResourcePmr(std::size_t n_resources, std::size_t initial_capacity);

    auto get_resource(std::size_t i) -> ArenaMemoryResourcePmr*;
};

template <typename... Types>
class MultiTArenaMemoryResourcePmr {
  private:
    using Indexer = types_to_index<Types...>;
    std::vector<ArenaMemoryResourcePmr> resources_;
  public:
    MultiTArenaMemoryResourcePmr(std::size_t initial_capacity) {
        resources_.reserve(Indexer::size());

        for (std::size_t i = 0; i < Indexer::size(); ++i) {
            resources_.emplace_back(initial_capacity);
        }
    }
    template <typename T>
    auto get_resource() -> ArenaMemoryResourcePmr* {
        return &resources_[Indexer::template get<T>()];
    }
};
}
