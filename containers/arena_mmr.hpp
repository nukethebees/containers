#pragma once

#include <cstddef>

#include "arena_memory_resource_pool.hpp"
#include "memory_resource_allocator.hpp"

namespace ml {
class arena_mmr {
  public:
    using size_type = std::size_t;

    arena_mmr() = default;
    explicit arena_mmr(size_type initial_capacity);
    ~arena_mmr();

    arena_mmr(arena_mmr const&) = delete;
    arena_mmr(arena_mmr&& other);

    auto operator=(arena_mmr const&) -> arena_mmr& = delete;
    auto operator=(arena_mmr&& other) -> arena_mmr&;

    // Access
    auto pool() const -> ArenaMemoryResourcePool const*;

    // Capacity
    auto initial_capacity() const -> size_type;
    auto n_pools() const -> size_type;
    auto total_size() const -> size_type;

    // Allocation
    auto allocate(size_type n_bytes, size_type alignment) -> void*;
    auto deallocate(void* ptr, size_type n_bytes, size_type alignment) -> void;
    // If there is room in the active pool then extend the allocation
    // otherwise create a new pool and allocate from it
    auto extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void*;
  private:
    ArenaMemoryResourcePool* pool_{nullptr};
    ArenaMemoryResourcePool* last_pool_{nullptr};
    size_type initial_capacity_{1024};
};

// Ctor
inline arena_mmr::~arena_mmr() {
    if (pool_) {
        pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte*>(pool_);
    }
}
// Access
inline auto arena_mmr::pool() const -> ArenaMemoryResourcePool const* {
    return pool_;
}
// Capacity
inline auto arena_mmr::initial_capacity() const -> size_type {
    return initial_capacity_;
}
inline auto arena_mmr::n_pools() const -> size_type {
    size_type count{0};
    for (auto const* p{pool()}; p; p = p->next_pool()) {
        ++count;
    }
    return count;
}
inline auto arena_mmr::total_size() const -> size_type {
    size_type total{0};
    for (auto const* p{pool()}; p; p = p->next_pool()) {
        total += p->size();
    }
    return total;
}
// Allocation
inline auto arena_mmr::deallocate(void* ptr, size_type n_bytes, size_type alignment) -> void {
    if (pool_) {
        pool_->deallocate(ptr, n_bytes, alignment);
    }
}
inline auto arena_mmr::extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* {
    if (!last_pool_) {
        return allocate(new_bytes, alignment);
    }

    if (new_bytes <= old_bytes) {
        return ptr;
    }

    auto* const pool{last_pool_};
    auto* const pool_cur_pos{pool->next_alloc_start()};
    auto const pool_size{pool->size()};

    // Do a size check to see if the allocation could have been made in the current pool
    if (old_bytes > pool_size) {
        return allocate(new_bytes, alignment);
    }

    // Check if the pointer matches up with the last pool allocation
    auto* const expected_start{pool_cur_pos - old_bytes};
    if (expected_start != ptr) {
        return allocate(new_bytes, alignment);
    }

    auto const alloc_diff{new_bytes - old_bytes};
    pool->extend_by(alloc_diff);
    return ptr;
}
}
