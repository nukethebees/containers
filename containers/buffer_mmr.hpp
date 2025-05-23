#pragma once

#include <array>
#include <cstddef>

#include "mmr_allocator.hpp"

namespace ml {
template <std::size_t CAPACITY>
class buffer_mmr {
  public:
    using size_type = std::size_t;

    buffer_mmr() = default;

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
    auto extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* {
        auto const extension_size{new_bytes - old_bytes};

        assert(new_bytes > old_bytes);

        if (extension_size > remaining_capacity_) {
            throw std::bad_alloc{};
        }

        // If it's null or a different allocation then do a fresh allocation
        if ((ptr == last_allocation_) && ptr) {
            remaining_capacity_ -= extension_size;
            return ptr;
        } else {
            return allocate(new_bytes, alignment);
        }
    }

    // Capacity
    [[nodiscard]] constexpr auto capacity() const -> size_type { return CAPACITY; }
    [[nodiscard]] auto remaining_capacity() const -> size_type { return remaining_capacity_; }
    [[nodiscard]] auto size() const -> size_type { return CAPACITY - remaining_capacity_; }
    [[nodiscard]] auto empty() const -> bool { return remaining_capacity_ == CAPACITY; }
  private:
    std::array<std::byte, CAPACITY> buffer;
    size_type remaining_capacity_{CAPACITY};
    void* last_allocation_{nullptr};
};

template <typename T, std::size_t CAPACITY>
struct StackAllocConfig {
    using Resource = buffer_mmr<CAPACITY * sizeof(T)>;
    using Allocator = mmr_allocator<T, Resource>;
};
}
