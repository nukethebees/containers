#include "multi_arena_resource.hpp"

namespace ml {
MultiArenaMemoryResourcePmr::MultiArenaMemoryResourcePmr(std::size_t n_resources, std::size_t initial_capacity) {
    resources_.reserve(n_resources);

    for (std::size_t i = 0; i < n_resources; ++i) {
        resources_.emplace_back(initial_capacity);
    }
}

auto MultiArenaMemoryResourcePmr::get_resource(std::size_t i) -> ArenaMemoryResourcePmr* {
    if (i >= resources_.size()) {
        throw std::out_of_range("Invalid resource index");
    }
    return &resources_[i];
}
}
