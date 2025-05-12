#pragma once

#include <cstddef>
#include <memory_resource>

#include "arena_memory_resource_pool.hpp"
#include "memory_resource_allocator.hpp"

namespace ml {
class ArenaMemoryResource {
  public:
    ArenaMemoryResource() = default;
    explicit ArenaMemoryResource(std::size_t initial_capacity);
    ~ArenaMemoryResource();

    ArenaMemoryResource(ArenaMemoryResource const&) = delete;
    ArenaMemoryResource(ArenaMemoryResource&& other);

    auto operator=(ArenaMemoryResource const&) -> ArenaMemoryResource& = delete;
    auto operator=(ArenaMemoryResource&& other) -> ArenaMemoryResource&;

    // Access
    auto pool() const -> ArenaMemoryResourcePool const*;

    // Capacity
    auto initial_capacity() const -> std::size_t;
    auto n_pools() const -> std::size_t;
    auto total_size() const -> std::size_t;

    // Allocation
    auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    auto deallocate(void* ptr, std::size_t n_bytes, std::size_t alignment) -> void;
    // If there is room in the active pool then extend the allocation
    // otherwise create a new pool and allocate from it
    auto extend(void* ptr, std::size_t old_bytes, std::size_t new_bytes, std::size_t alignment) -> void*;
  private:
    ArenaMemoryResourcePool* pool_{nullptr};
    ArenaMemoryResourcePool* last_pool_{nullptr};
    std::size_t initial_capacity_{1024};
};

// Ctor
inline ArenaMemoryResource::~ArenaMemoryResource() {
    if (pool_) {
        pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte*>(pool_);
    }
}
// Access
inline auto ArenaMemoryResource::pool() const -> ArenaMemoryResourcePool const* {
    return pool_;
}
// Capacity
inline auto ArenaMemoryResource::initial_capacity() const -> std::size_t {
    return initial_capacity_;
}
inline auto ArenaMemoryResource::n_pools() const -> std::size_t {
    std::size_t count{0};
    for (auto const* p{pool()}; p; p = p->next_pool()) {
        ++count;
    }
    return count;
}
inline auto ArenaMemoryResource::total_size() const -> std::size_t {
    std::size_t total{0};
    for (auto const* p{pool()}; p; p = p->next_pool()) {
        total += p->size();
    }
    return total;
}
// Allocation
inline auto ArenaMemoryResource::deallocate(void* ptr, std::size_t n_bytes, std::size_t alignment) -> void {
    if (pool_) {
        pool_->deallocate(ptr, n_bytes, alignment);
    }
}
inline auto ArenaMemoryResource::extend(void* ptr, std::size_t old_bytes, std::size_t new_bytes, std::size_t alignment)
    -> void* {
    if (!last_pool_) {
        return allocate(new_bytes, alignment);
    }

    auto* const pool{last_pool_};
    auto* const pool_cur_pos{pool->next_alloc_start()};
    auto const pool_size{pool->size()};

    // Do a size check to see if the allocation could have been made in the current pool
    if (pool_size < new_bytes) {
        return allocate(new_bytes, alignment);
    }

    // Check if the pointer matches up with the last pool allocation
    auto* const expected_start{pool_cur_pos - old_bytes};
    if (expected_start != ptr) {
        return allocate(new_bytes, alignment);
    }

    return nullptr;
}

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;

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
    ArenaMemoryResource arena;
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
