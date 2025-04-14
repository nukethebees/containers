#include <cmath>

#include "arena_memory_resource.hpp"
#include "misc.hpp"

namespace ml {
ArenaMemoryResourcePool::ArenaMemoryResourcePool(std::size_t capacity)
    : total_capacity_{capacity}
    , remaining_capacity_{capacity} {}

ArenaMemoryResourcePool::~ArenaMemoryResourcePool() {
    if (next_pool_) {
        next_pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte *>(next_pool_);
    }
}

auto ArenaMemoryResourcePool::create_pool(std::size_t initial_size) -> ArenaMemoryResourcePool * {
    std::size_t const bytes_needed{initial_size + sizeof(ArenaMemoryResourcePool)};
    auto * buffer{new std::byte[bytes_needed]};
    return new (buffer) ArenaMemoryResourcePool(initial_size);
}

auto ArenaMemoryResourcePool::next_pool() const -> ArenaMemoryResourcePool const * {
    return next_pool_;
}
auto ArenaMemoryResourcePool::total_capacity() const -> std::size_t {
    return total_capacity_;
}
auto ArenaMemoryResourcePool::remaining_capacity() const -> std::size_t {
    return remaining_capacity_;
}
auto ArenaMemoryResourcePool::size() const -> std::size_t {
    return total_capacity_ - remaining_capacity_;
}
auto ArenaMemoryResourcePool::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    auto const cur_size{size()};

    static constexpr std::size_t this_size{
        sizeof(std::remove_cvref_t<decltype(*this)>)};

    auto * new_start{static_cast<void *>(
        static_cast<std::byte *>(static_cast<void *>(this))
        + this_size
        + cur_size)};

    if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
        throw std::bad_alloc{};
    }

    remaining_capacity_ -= n_bytes;
    return new_start;
}
void ArenaMemoryResourcePool::deallocate(void * alloc, std::size_t n_bytes, std::size_t alignment) {
    if (next_pool_) {
        next_pool_->deallocate(alloc, n_bytes, alignment);
    }
    return;
}

ArenaMemoryResource::ArenaMemoryResource(std::size_t initial_capacity)
    : initial_capacity_{initial_capacity} {}
ArenaMemoryResource::~ArenaMemoryResource() {
    if (pool_) {
        pool_->~ArenaMemoryResourcePool();
        delete[] reinterpret_cast<std::byte *>(pool_);
    }
}
ArenaMemoryResource::ArenaMemoryResource(ArenaMemoryResource && other)
    : pool_{other.pool_}
    , last_pool_{other.last_pool_} {
    other.pool_ = nullptr;
    other.last_pool_ = nullptr;
}

auto ArenaMemoryResource::operator=(ArenaMemoryResource && other) -> ArenaMemoryResource & {
    if (this != &other) {
        if (pool_) {
            pool_->~ArenaMemoryResourcePool();
            delete[] reinterpret_cast<std::byte *>(pool_);
        }
        pool_ = other.pool_;
        last_pool_ = other.last_pool_;
        other.pool_ = nullptr;
        other.last_pool_ = nullptr;
    }
    return *this;
}

auto ArenaMemoryResource::initial_capacity() const -> std::size_t {
    return initial_capacity_;
}
auto ArenaMemoryResource::pool() const -> ArenaMemoryResourcePool const * {
    return pool_;
}
auto ArenaMemoryResource::n_pools() const -> std::size_t {
    std::size_t count{0};
    for (auto const * p{pool()}; p; p = p->next_pool()) {
        ++count;
    }
    return count;
}
auto ArenaMemoryResource::total_size() const -> std::size_t {
    std::size_t total{0};
    for (auto const * p{pool()}; p; p = p->next_pool()) {
        total += p->size();
    }
    return total;
}
auto ArenaMemoryResource::allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
    if (!last_pool_) {
        auto const cap{initial_capacity_};
        auto const new_size{ml::max(cap * 2, (n_bytes / cap) * 2 * cap)};
        pool_ = ArenaMemoryResourcePool::create_pool(new_size);
        last_pool_ = pool_;
        return last_pool_->allocate(n_bytes, alignment);
    }

    if (last_pool_->remaining_capacity() <= n_bytes) {
        auto const cap{last_pool_->total_capacity()};
        auto const new_size{ml::max(cap * 2, (n_bytes / cap) * 2 * cap)};
        last_pool_->next_pool_ = ArenaMemoryResourcePool::create_pool(new_size);
        last_pool_ = last_pool_->next_pool_;
        return last_pool_->allocate(n_bytes, alignment);
    }

    return last_pool_->allocate(n_bytes, alignment);
}
auto ArenaMemoryResource::deallocate(void * ptr, std::size_t n_bytes, std::size_t alignment) -> void {
    if (pool_) {
        pool_->deallocate(ptr, n_bytes, alignment);
    }
}
}
