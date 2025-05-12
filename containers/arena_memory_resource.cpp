#include <cmath>

#include "arena_memory_resource.hpp"
#include "misc.hpp"

namespace ml {
ArenaMemoryResourcePool::ArenaMemoryResourcePool(std::size_t capacity)
    : total_capacity_{capacity}
    , remaining_capacity_{capacity} {}

ArenaMemoryResource::ArenaMemoryResource(std::size_t initial_capacity)
    : initial_capacity_{initial_capacity} {}
ArenaMemoryResource::ArenaMemoryResource(ArenaMemoryResource&& other)
    : pool_{other.pool_}
    , last_pool_{other.last_pool_} {
    other.pool_ = nullptr;
    other.last_pool_ = nullptr;
}

auto ArenaMemoryResource::operator=(ArenaMemoryResource&& other) -> ArenaMemoryResource& {
    if (this != &other) {
        if (pool_) {
            pool_->~ArenaMemoryResourcePool();
            delete[] reinterpret_cast<std::byte*>(pool_);
        }
        pool_ = other.pool_;
        last_pool_ = other.last_pool_;
        other.pool_ = nullptr;
        other.last_pool_ = nullptr;
    }
    return *this;
}

auto ArenaMemoryResource::allocate(std::size_t n_bytes, std::size_t alignment) -> void* {
    auto make_new_size{[](auto cap, auto n_b) { return ml::max(cap, (n_b / cap) * std::size_t{2} * cap); }};

    if (!last_pool_) {
        auto const cap{initial_capacity_};
        pool_ = ArenaMemoryResourcePool::create_pool(make_new_size(cap, n_bytes));
        last_pool_ = pool_;
    } else if (last_pool_->remaining_capacity() <= n_bytes) {
        auto const cap{last_pool_->total_capacity()};
        last_pool_->next_pool_ = ArenaMemoryResourcePool::create_pool(make_new_size(cap, n_bytes));
        last_pool_ = last_pool_->next_pool_;
    }

    return last_pool_->allocate(n_bytes, alignment);
}

}
