#pragma once

#include <cstddef>
#include <memory_resource>

#include "arena_mmr.hpp"

namespace ml {
class ArenaMemoryResourcePmr : public std::pmr::memory_resource {
  public:
    ArenaMemoryResourcePmr() = default;
    explicit ArenaMemoryResourcePmr(std::size_t initial_capacity);
    ~ArenaMemoryResourcePmr() override = default;

    ArenaMemoryResourcePmr(ArenaMemoryResourcePmr const&) = delete;
    ArenaMemoryResourcePmr(ArenaMemoryResourcePmr&& other) noexcept = default;

    auto operator=(ArenaMemoryResourcePmr const&) -> ArenaMemoryResourcePmr& = delete;
    auto operator=(ArenaMemoryResourcePmr&& other) noexcept -> ArenaMemoryResourcePmr& = default;
  protected:
    auto do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* override final;
    void do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) override final;
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool override final;
  private:
    arena_mmr arena;
};

// Ctor
inline ArenaMemoryResourcePmr::ArenaMemoryResourcePmr(std::size_t initial_capacity)
    : arena{initial_capacity} {}
// Methods
inline auto ArenaMemoryResourcePmr::do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* {
    return arena.allocate(n_bytes, alignment);
}
inline void ArenaMemoryResourcePmr::do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) {
    return arena.deallocate(p, n_bytes, alignment);
}
inline auto ArenaMemoryResourcePmr::do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool {
    return this == &other;
}
}
