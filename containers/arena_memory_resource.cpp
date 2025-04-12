#include <cmath>

#include "arena_memory_resource.hpp"
#include "misc.hpp"

namespace ml {

ArenaMemoryResource::Pool::Pool(std::size_t capacity)
    : buffer{new std::byte[capacity]}
    , total_capacity_{capacity}
    , remaining_capacity_{capacity}
    , last_allocation_{buffer.get()} {}

auto ArenaMemoryResource::Pool::total_capacity() const -> std::size_t {
    return total_capacity_;
}
auto ArenaMemoryResource::Pool::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
auto ArenaMemoryResource::Pool::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}
auto ArenaMemoryResource::Pool::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    auto const cur_size{size()};

    auto * new_start{static_cast<void *>(buffer.get() + cur_size)};
    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }

    remaining_capacity_ -= n_bytes;
    this->last_allocation_ = new_start;
    return this->last_allocation_;
}
auto ArenaMemoryResource::Pool::reallocate(void * alloc, std::size_t new_size_bytes, std::size_t alignment) -> void * {
    if (alloc != last_allocation_) {
        throw std::bad_alloc{};
    }

    auto * const byte_ptr{static_cast<std::byte *>(alloc)};
    auto const current_alloc_offset{byte_ptr - buffer.get()};
    this->remaining_capacity_ = this->total_capacity_ - current_alloc_offset;

    return allocate(new_size_bytes, alignment);
}

ArenaMemoryResource::ArenaMemoryResource(std::size_t initial_capacity) noexcept
    : initial_capacity_{initial_capacity} {}

auto ArenaMemoryResource::get_sufficient_pool(std::size_t n_bytes) -> Pool & {
    if (pools_.empty()) {
        if (n_bytes > initial_capacity_) {
            pools_.emplace_back(n_bytes);
        } else {
            pools_.emplace_back(initial_capacity_);
        }
        return pools_.back();
    }

    auto & pool{pools_.back()};

    if (pool.remaining_capacity() >= n_bytes) {
        return pool;
    }

    auto const cap{pool.total_capacity()};
    std::size_t const new_capacity{ml::max(cap * 2, (n_bytes / cap) * 2)};
    pools_.emplace_back(new_capacity);
    return pools_.back();
}
auto ArenaMemoryResource::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    auto & pool{get_sufficient_pool(n_bytes)};
    return pool.allocate(n_bytes, alignment);
}
auto ArenaMemoryResource::reallocate(void * alloc, std::size_t n_bytes, std::size_t alignment) -> void * {
    if (pools_.empty()) {
        return this->allocate(n_bytes, alignment);
    }
    auto & pool{pools_.back()};
    return pool.reallocate(alloc, n_bytes, alignment);
}

auto ArenaMemoryResource::initial_capacity() const -> std::size_t {
    return initial_capacity_;
}

Pool2::Pool2(std::byte * buffer, std::size_t capacity)
    : buffer{buffer}
    , total_capacity_{capacity}
    , remaining_capacity_{capacity} {}

Pool2::~Pool2() {
    if (next_pool_) {
        next_pool_->~Pool2();
        delete[] reinterpret_cast<std::byte *>(next_pool_);
    }
}

auto Pool2::create_pool(std::size_t initial_size) -> Pool2 * {
    std::size_t const bytes_needed{initial_size + sizeof(Pool2)};
    auto * buffer{new std::byte[bytes_needed]};
    return new (buffer) Pool2(buffer + sizeof(Pool2), initial_size);
}

auto Pool2::next_pool() const -> Pool2 const * {
    return next_pool_;
}
auto Pool2::total_capacity() const -> std::size_t {
    return total_capacity_;
}
auto Pool2::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
auto Pool2::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}
auto Pool2::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    auto const cur_size{size()};

    if (n_bytes > remaining_capacity_) {
        if (next_pool_) {
            return next_pool_->allocate(n_bytes, alignment);
        }
        auto * new_pool{create_pool(std::max(n_bytes, total_capacity_ * 2))};
        next_pool_ = new_pool;
        return new_pool->allocate(n_bytes, alignment);
    }

    auto * new_start{static_cast<void *>(buffer + cur_size)};
    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }
    remaining_capacity_ -= n_bytes;
    return new_start;
}
void Pool2::deallocate(void * alloc, std::size_t n_bytes, std::size_t alignment) {
    if (next_pool_) {
        next_pool_->deallocate(alloc, n_bytes, alignment);
    }
    return;
}

ArenaMemoryResource2::ArenaMemoryResource2(std::size_t initial_capacity)
    : initial_capacity_{initial_capacity} {}

ArenaMemoryResource2::~ArenaMemoryResource2() {
    if (pool_) {
        pool_->~Pool2();
        delete[] reinterpret_cast<std::byte *>(pool_);
    }
}

ArenaMemoryResource2::ArenaMemoryResource2(ArenaMemoryResource2 && other)
    : pool_{other.pool_} {
    other.pool_ = nullptr;
}

auto ArenaMemoryResource2::operator=(ArenaMemoryResource2 && other) -> ArenaMemoryResource2 & {
    if (this != &other) {
        if (pool_) {
            pool_->~Pool2();
            delete[] reinterpret_cast<std::byte *>(pool_);
        }
        pool_ = other.pool_;
        other.pool_ = nullptr;
    }
    return *this;
}

auto ArenaMemoryResource2::initial_capacity() const -> std::size_t {
    return initial_capacity_;
}
auto ArenaMemoryResource2::pool() const -> Pool2 const * {
    return pool_;
}
auto ArenaMemoryResource2::n_pools() const -> std::size_t {
    std::size_t count{0};
    for (auto const * p{pool()}; p; p = p->next_pool()) {
        ++count;
    }
    return count;
}
auto ArenaMemoryResource2::total_size() const -> std::size_t {
    std::size_t total{0};
    for (auto const * p{pool()}; p; p = p->next_pool()) {
        total += p->size();
    }
    return total;
}
auto ArenaMemoryResource2::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    if (!pool_) {
        create_pool();
    }
    return pool_->allocate(n_bytes, alignment);
}
auto ArenaMemoryResource2::deallocate(void * ptr, std::size_t n_bytes, std::size_t alignment) -> void {
    if (pool_) {
        pool_->deallocate(ptr, n_bytes, alignment);
    }
}

void ArenaMemoryResource2::create_pool() {
    pool_ = Pool2::create_pool(initial_capacity_);
}

}
