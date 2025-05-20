#pragma once

#include <cstddef>
#include <memory_resource>

#include "arena_mmr.hpp"

namespace ml {
class arena_pmr : public std::pmr::memory_resource {
  public:
    arena_pmr() = default;
    explicit arena_pmr(std::size_t initial_capacity);
    ~arena_pmr() override = default;

    arena_pmr(arena_pmr const&) = delete;
    arena_pmr(arena_pmr&& other) noexcept = default;

    auto operator=(arena_pmr const&) -> arena_pmr& = delete;
    auto operator=(arena_pmr&& other) noexcept -> arena_pmr& = default;
  protected:
    auto do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* override final;
    void do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) override final;
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool override final;
  private:
    arena_mmr arena;
};

// Ctor
inline arena_pmr::arena_pmr(std::size_t initial_capacity)
    : arena{initial_capacity} {}
// Methods
inline auto arena_pmr::do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* {
    return arena.allocate(n_bytes, alignment);
}
inline void arena_pmr::do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) {
    return arena.deallocate(p, n_bytes, alignment);
}
inline auto arena_pmr::do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool {
    return this == &other;
}
}
