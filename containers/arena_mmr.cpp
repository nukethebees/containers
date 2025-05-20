#include <cmath>

#include "arena_mmr.hpp"
#include "misc.hpp"

namespace ml {
arena_mmr::arena_mmr(size_type initial_capacity)
    : initial_capacity_{initial_capacity} {}
arena_mmr::arena_mmr(arena_mmr&& other)
    : pool_{other.pool_}
    , last_pool_{other.last_pool_} {
    other.pool_ = nullptr;
    other.last_pool_ = nullptr;
}

auto arena_mmr::operator=(arena_mmr&& other) -> arena_mmr& {
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

auto arena_mmr::allocate(size_type n_bytes, size_type alignment) -> void* {
    auto make_new_size{[](auto cap, auto n_b) { return ml::max(cap, (n_b / cap) * size_type{2} * cap); }};

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
