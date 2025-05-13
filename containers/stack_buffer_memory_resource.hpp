#pragma once

#include <array>
#include <cstddef>

#include "memory_resource_allocator.hpp"

namespace ml {
template <std::size_t CAPACITY>
class StackBufferMemoryResource {
  public:
    using size_type = std::size_t;

    StackBufferMemoryResource() = default;

    // Allocation
    [[nodiscard]] auto allocate(size_type n_bytes, size_type alignment) -> void* {
        auto const cur_size{CAPACITY - remaining_capacity_};

        auto* new_start{static_cast<void*>(buffer.data() + cur_size)};
        if (!std::align(alignment, n_bytes, new_start, remaining_capacity_)) {
            throw std::bad_alloc{};
        }

        remaining_capacity_ -= n_bytes;
        this->last_allocation_ = new_start;
        return this->last_allocation_;
    }
    void deallocate(void* /*alloc*/, size_type /*n_bytes*/, size_type /*alignment*/) { return; }
    auto extend(void* /*ptr*/, size_type /*old_bytes*/, size_type /*new_bytes*/, size_type /*alignment*/) -> void* {
        return nullptr;
    }

    // Capacity
    [[nodiscard]] auto remaining_capacity() const -> size_type { return remaining_capacity_; }
    [[nodiscard]] auto size() const -> size_type { return CAPACITY - remaining_capacity_; }
  private:
    std::array<std::byte, CAPACITY> buffer;
    size_type remaining_capacity_{CAPACITY};
    void* last_allocation_{nullptr};
};

template <typename T, std::size_t CAPACITY>
struct StackAllocConfig {
    using Resource = StackBufferMemoryResource<CAPACITY * sizeof(T)>;
    using Allocator = MemoryResourceAllocator<T, Resource>;
};
}
