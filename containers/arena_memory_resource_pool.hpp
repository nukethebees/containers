#pragma once

#include <cstddef>

namespace ml {
class ArenaMemoryResource;

class ArenaMemoryResourcePool {
  public:
    ArenaMemoryResourcePool(std::size_t capacity);
    ~ArenaMemoryResourcePool();

    friend class ArenaMemoryResource;

    static auto create_pool(std::size_t initial_size) -> ArenaMemoryResourcePool*;

    // Access
    auto next_pool() const -> ArenaMemoryResourcePool const*;

    // Capacity
    auto total_capacity() const -> std::size_t;
    auto remaining_capacity() const -> std::size_t;
    auto size() const -> std::size_t;

    // Allocation
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    void deallocate(void* alloc, std::size_t n_bytes, std::size_t alignment);
  private:
    ArenaMemoryResourcePool* next_pool_{nullptr};
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
};

// Ctor
inline ArenaMemoryResourcePool::ArenaMemoryResourcePool(std::size_t capacity)
    : total_capacity_{capacity}
    , remaining_capacity_{capacity} {}
inline ArenaMemoryResourcePool::~ArenaMemoryResourcePool() {
    if (next_pool_) {
        next_pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte*>(next_pool_);
    }
}
// Access
inline auto ArenaMemoryResourcePool::next_pool() const -> ArenaMemoryResourcePool const* {
    return next_pool_;
}
// Capacity
inline auto ArenaMemoryResourcePool::total_capacity() const -> std::size_t {
    return total_capacity_;
}
inline auto ArenaMemoryResourcePool::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
inline auto ArenaMemoryResourcePool::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}
// Allocation
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
}
