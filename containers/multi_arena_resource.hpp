#pragma once

#include <cstddef>
#include <vector>

#include "arena_memory_resource.hpp"

namespace ml {
class MultiArenaMemoryResourcePmr {
  private:
    std::vector<ArenaMemoryResourcePmr> resources_;
  public:
    MultiArenaMemoryResourcePmr() = delete;
    MultiArenaMemoryResourcePmr(std::size_t n_resources, std::size_t initial_capacity);

    auto get_resource(std::size_t i) -> ArenaMemoryResourcePmr*;
};
}
