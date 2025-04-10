#include <cmath>

#include "arena_allocator.hpp"
#include "misc.hpp"

namespace ml {
ArenaMemoryResource::Pool::Pool(Size capacity)
    : buffer{static_cast<std::byte*>(::operator new[](capacity))}
    , total_capacity_{capacity}
    , remaining_capacity_{capacity}
    , last_allocation_{buffer} {}
ArenaMemoryResource::Pool::~Pool() {
    ::operator delete[](static_cast<void*>(buffer));
}

ArenaMemoryResource::Pool::Pool(Pool const& other)
    : Pool{other.total_capacity()} {}
ArenaMemoryResource::Pool::Pool(Pool&& other) noexcept
    : buffer{other.buffer}
    , total_capacity_{other.total_capacity_}
    , remaining_capacity_{other.remaining_capacity_}
    , last_allocation_{other.last_allocation_} {
    other.buffer = nullptr;
    other.total_capacity_ = 0;
    other.remaining_capacity_ = 0;
    other.last_allocation_ = nullptr;
}

auto ArenaMemoryResource::Pool::operator=(Pool const& other) -> Pool& {
    if (this != &other) {
        this->~Pool();
        new (this) Pool{other};
    }
    return *this;
}
auto ArenaMemoryResource::Pool::operator=(Pool&& other) noexcept -> Pool& {
    if (this != &other) {
        this->~Pool();
        new (this) Pool{std::move(other)};
    }
    return *this;
}

auto ArenaMemoryResource::Pool::total_capacity() const -> Size {
    return total_capacity_;
}
auto ArenaMemoryResource::Pool::remaining_capacity() const -> Size {
    return remaining_capacity_;
}
auto ArenaMemoryResource::Pool::size() const -> Size {
    return total_capacity_ - remaining_capacity_;
}
auto ArenaMemoryResource::Pool::allocate(Size n_bytes, std::size_t alignment) -> void* {
    auto const cur_size{size()};

    auto* new_start{static_cast<void*>(buffer + cur_size)};
    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }

    remaining_capacity_ -= n_bytes;
    this->last_allocation_ = new_start;
    return this->last_allocation_;
}
auto ArenaMemoryResource::Pool::reallocate(void* alloc, Size new_size_bytes, std::size_t alignment) -> void* {
    if (alloc != last_allocation_) {
        throw std::bad_alloc{};
    }

    // 00000000000000000000000
    // ^     ^       ^
    // B     A       S
    // B = buffer start
    // A = allocation start
    // S = current end pointer

    auto* const byte_ptr{static_cast<std::byte*>(alloc)};
    auto const current_alloc_offset{byte_ptr - buffer};
    this->remaining_capacity_ = this->total_capacity_ - current_alloc_offset;

    return allocate(new_size_bytes, alignment);
}

ArenaMemoryResource::ArenaMemoryResource(Size initial_capacity) noexcept
    : initial_capacity_{initial_capacity} {}

auto ArenaMemoryResource::get_sufficient_pool(Size n_bytes) -> Pool& {
    if (pools_.empty()) {
        if (n_bytes > initial_capacity_) {
            pools_.emplace_back(n_bytes);
        } else {
            pools_.emplace_back(initial_capacity_);
        }
        return pools_.back();
    }

    auto& pool{pools_.back()};

    if (pool.remaining_capacity() >= n_bytes) {
        return pool;
    }

    auto const cap{pool.total_capacity()};
    std::size_t const new_capacity{ml::max(cap * 2, (n_bytes / cap) * 2)};
    pools_.emplace_back(new_capacity);
    return pools_.back();
}
auto ArenaMemoryResource::allocate(Size n_bytes, std::size_t alignment) -> void* {
    auto& pool{get_sufficient_pool(n_bytes)};
    return pool.allocate(n_bytes, alignment);
}
auto ArenaMemoryResource::reallocate(void* alloc, Size n_bytes, std::size_t alignment) -> void* {
    if (pools_.empty()) {
        return this->allocate(n_bytes, alignment);
    }
    auto& pool{pools_.back()};
    return pool.reallocate(alloc, n_bytes, alignment);
}

auto ArenaMemoryResource::initial_capacity() const -> Size {
    return initial_capacity_;
}
}
