#pragma once

#include <cstddef>
#include <memory_resource>

#include "memory_resource_allocator.hpp"

namespace ml {
class ArenaMemoryResource;

class ArenaMemoryResourcePool {
  private:
    ArenaMemoryResourcePool* next_pool_{nullptr};
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
  public:
    ArenaMemoryResourcePool(std::size_t capacity);
    ~ArenaMemoryResourcePool();

    friend class ArenaMemoryResource;

    static auto create_pool(std::size_t initial_size) -> ArenaMemoryResourcePool*;

    auto next_pool() const -> ArenaMemoryResourcePool const*;
    auto total_capacity() const -> std::size_t;
    auto remaining_capacity() const -> std::size_t;
    auto size() const -> std::size_t;
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    void deallocate(void* alloc, std::size_t n_bytes, std::size_t alignment);
};

inline ArenaMemoryResourcePool::~ArenaMemoryResourcePool() {
    if (next_pool_) {
        next_pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte*>(next_pool_);
    }
}

inline auto ArenaMemoryResourcePool::next_pool() const -> ArenaMemoryResourcePool const* {
    return next_pool_;
}
inline auto ArenaMemoryResourcePool::total_capacity() const -> std::size_t {
    return total_capacity_;
}
inline auto ArenaMemoryResourcePool::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
inline auto ArenaMemoryResourcePool::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}

inline auto ArenaMemoryResourcePool::create_pool(std::size_t initial_size) -> ArenaMemoryResourcePool* {
    std::size_t const bytes_needed{initial_size + sizeof(ArenaMemoryResourcePool)};
    auto* buffer{new std::byte[bytes_needed]};
    return new (buffer) ArenaMemoryResourcePool(initial_size);
}
inline auto ArenaMemoryResourcePool::allocate(std::size_t n_bytes, std::size_t alignment) -> void* {
    auto const cur_size{size()};

    static constexpr std::size_t this_size{sizeof(std::remove_cvref_t<decltype(*this)>)};

    auto* new_start{static_cast<void*>(static_cast<std::byte*>(static_cast<void*>(this)) + this_size + cur_size)};

    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }

    remaining_capacity_ -= n_bytes;
    return new_start;
}
inline void ArenaMemoryResourcePool::deallocate(void* /*alloc*/, std::size_t /*n_bytes*/, std::size_t /*alignment*/) {
    // no-op
    return;
}

class ArenaMemoryResource {
  private:
    ArenaMemoryResourcePool* pool_{nullptr};
    ArenaMemoryResourcePool* last_pool_{nullptr};
    std::size_t initial_capacity_{1024};
  public:
    ArenaMemoryResource() = default;
    explicit ArenaMemoryResource(std::size_t initial_capacity);
    ~ArenaMemoryResource();

    ArenaMemoryResource(ArenaMemoryResource const&) = delete;
    ArenaMemoryResource(ArenaMemoryResource&& other);
    auto operator=(ArenaMemoryResource const&) -> ArenaMemoryResource& = delete;
    auto operator=(ArenaMemoryResource&& other) -> ArenaMemoryResource&;

    auto initial_capacity() const -> std::size_t;
    auto pool() const -> ArenaMemoryResourcePool const*;
    auto n_pools() const -> std::size_t;
    auto total_size() const -> std::size_t;
    auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    auto deallocate(void* ptr, std::size_t n_bytes, std::size_t alignment) -> void;
};

inline ArenaMemoryResource::~ArenaMemoryResource() {
    if (pool_) {
        pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte*>(pool_);
    }
}
inline auto ArenaMemoryResource::initial_capacity() const -> std::size_t {
    return initial_capacity_;
}
inline auto ArenaMemoryResource::pool() const -> ArenaMemoryResourcePool const* {
    return pool_;
}
inline auto ArenaMemoryResource::n_pools() const -> std::size_t {
    std::size_t count{ 0 };
    for (auto const* p{ pool() }; p; p = p->next_pool()) {
        ++count;
    }
    return count;
}
inline auto ArenaMemoryResource::total_size() const -> std::size_t {
    std::size_t total{ 0 };
    for (auto const* p{ pool() }; p; p = p->next_pool()) {
        total += p->size();
    }
    return total;
}
inline auto ArenaMemoryResource::deallocate(void* ptr, std::size_t n_bytes, std::size_t alignment) -> void {
    if (pool_) {
        pool_->deallocate(ptr, n_bytes, alignment);
    }
}

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;

class ArenaMemoryResourcePmr : public std::pmr::memory_resource {
  private:
    ArenaMemoryResource arena;
  public:
    ArenaMemoryResourcePmr() = default;
    explicit ArenaMemoryResourcePmr(std::size_t initial_capacity);
    ~ArenaMemoryResourcePmr() override = default;

    ArenaMemoryResourcePmr(ArenaMemoryResourcePmr const&) = default;
    ArenaMemoryResourcePmr(ArenaMemoryResourcePmr&& other) noexcept = default;

    auto operator=(ArenaMemoryResourcePmr const&) -> ArenaMemoryResourcePmr& = default;
    auto operator=(ArenaMemoryResourcePmr&& other) noexcept -> ArenaMemoryResourcePmr& = default;
  protected:
    auto do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* override final;
    void do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) override final;
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool override final;
};

inline ArenaMemoryResourcePmr::ArenaMemoryResourcePmr(std::size_t initial_capacity)
    : arena{initial_capacity} {}
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
