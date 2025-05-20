#pragma once

#include <cstddef>
#include <vector>

#include "arena_mmr.hpp"
#include "arena_pmr.hpp"
#include "misc.hpp"

namespace ml {
class multi_arena_pmr {
  private:
    std::vector<arena_pmr> resources_;
  public:
    multi_arena_pmr() = delete;
    multi_arena_pmr(std::size_t n_resources, std::size_t initial_capacity);

    auto get_resource(std::size_t i) -> arena_pmr*;
};

template <typename... Types>
class multi_t_arena_pmr {
  private:
    using Indexer = types_to_index<Types...>;
    std::vector<arena_pmr> resources_;
  public:
    multi_t_arena_pmr(std::size_t initial_capacity) {
        resources_.reserve(Indexer::size());

        for (std::size_t i = 0; i < Indexer::size(); ++i) {
            resources_.emplace_back(initial_capacity);
        }
    }
    template <typename T>
    auto get_resource() -> arena_pmr* {
        return &resources_[Indexer::template get<T>()];
    }
};
}
