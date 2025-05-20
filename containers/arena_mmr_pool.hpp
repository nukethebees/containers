#pragma once

#include <cstddef>
#include <memory>

namespace ml {
class arena_mmr;

class arena_mmr_pool {
  public:
    arena_mmr_pool(std::size_t capacity);
    ~arena_mmr_pool();

    friend class arena_mmr;

    static auto create_pool(std::size_t initial_size) -> arena_mmr_pool*;

    // Access
    auto next_pool() const -> arena_mmr_pool const*;
    auto data() -> std::byte*;
    // Points to the address after the last byte of the pool
    auto data_end() -> std::byte*;
    auto next_alloc_start() -> std::byte*;

    // Capacity
    auto total_capacity() const -> std::size_t;
    auto remaining_capacity() const -> std::size_t;
    auto size() const -> std::size_t;

    // Allocation
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    void deallocate(void* alloc, std::size_t n_bytes, std::size_t alignment);
    void extend_by(std::size_t n_bytes);
  private:
    arena_mmr_pool* next_pool_{nullptr};
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
};

// Ctor
inline arena_mmr_pool::arena_mmr_pool(std::size_t capacity)
    : total_capacity_{capacity}
    , remaining_capacity_{capacity} {}
inline arena_mmr_pool::~arena_mmr_pool() {
    if (next_pool_) {
        next_pool_->~arena_mmr_pool();
        delete[] reinterpret_cast<std::byte*>(next_pool_);
    }
}
// Access
inline auto arena_mmr_pool::next_pool() const -> arena_mmr_pool const* {
    return next_pool_;
}
inline auto arena_mmr_pool::data() -> std::byte* {
    return static_cast<std::byte*>(static_cast<void*>(this)) + sizeof(arena_mmr_pool);
}
inline auto arena_mmr_pool::data_end() -> std::byte* {
    return data() + total_capacity_;
}
inline auto arena_mmr_pool::next_alloc_start() -> std::byte* {
    return data() + size();
}
// Capacity
inline auto arena_mmr_pool::total_capacity() const -> std::size_t {
    return total_capacity_;
}
inline auto arena_mmr_pool::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
inline auto arena_mmr_pool::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}
// Allocation
inline auto arena_mmr_pool::create_pool(std::size_t initial_size) -> arena_mmr_pool* {
    std::size_t const bytes_needed{initial_size + sizeof(arena_mmr_pool)};
    auto* buffer{new std::byte[bytes_needed]};
    return new (buffer) arena_mmr_pool(initial_size);
}
inline auto arena_mmr_pool::allocate(std::size_t n_bytes, std::size_t alignment) -> void* {
    auto* new_start{static_cast<void*>(next_alloc_start())};

    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }

    remaining_capacity_ -= n_bytes;
    return new_start;
}
inline void arena_mmr_pool::deallocate(void* /*alloc*/, std::size_t /*n_bytes*/, std::size_t /*alignment*/) {
    // no-op
    return;
}
inline void arena_mmr_pool::extend_by(std::size_t n_bytes) {
    remaining_capacity_ -= n_bytes;
}
}
