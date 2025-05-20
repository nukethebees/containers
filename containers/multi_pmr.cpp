#include <stdexcept>

#include "multi_pmr.hpp"

namespace ml {
multi_arena_pmr::multi_arena_pmr(std::size_t n_resources, std::size_t initial_capacity) {
    resources_.reserve(n_resources);

    for (std::size_t i = 0; i < n_resources; ++i) {
        resources_.emplace_back(initial_capacity);
    }
}

auto multi_arena_pmr::get_resource(std::size_t i) -> arena_pmr* {
    if (i >= resources_.size()) {
        throw std::out_of_range("Invalid resource index");
    }
    return &resources_[i];
}
}
