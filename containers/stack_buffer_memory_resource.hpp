#pragma once

#include <array>
#include <cstddef>

#include "memory_resource_allocator.hpp"

namespace ml {
template <std::size_t CAPACITY>
class StackBufferMemoryResource {
    std::array<std::byte, CAPACITY> buffer;
    std::size_t remaining_capacity_{CAPACITY};
    void * last_allocation_{nullptr};
public:
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void * {
        auto const cur_size{CAPACITY - remaining_capacity_};

        auto * new_start{static_cast<void *>(buffer.data() + cur_size)};
        if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
            throw std::bad_alloc{};
        }

        remaining_capacity_ -= n_bytes;
        this->last_allocation_ = new_start;
        return this->last_allocation_;
    }
    [[nodiscard]] auto remaining_capacity() const -> std::size_t {
        return remaining_capacity_;
    }
    [[nodiscard]] auto size() const -> std::size_t {
        return CAPACITY - remaining_capacity_;
    }
};

template <typename T, std::size_t CAPACITY>
struct StackAllocConfig {
    using Resource = StackBufferMemoryResource<CAPACITY * sizeof(T)>;
    using Allocator = MemoryResourceAllocator<T, Resource>;
};
}