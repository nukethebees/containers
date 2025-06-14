#pragma once

#include <type_traits>

#include "memory_resource_concepts.hpp"
#include "buffer_mmr.hpp"

namespace ml {
template <typename T, std::size_t CAPACITY_ELEMS, typename resource_base>
class buffer_pmr : public resource_base {
  public:
    static inline constexpr bool resource_can_extend{extendable_memory_resource<resource_base>};
    static inline constexpr auto CAPACITY_BYTES{CAPACITY_ELEMS * sizeof(T)};

    using size_type = std::size_t;

    buffer_pmr() = default;
    ~buffer_pmr() override = default;

    auto do_allocate(size_type n_bytes, size_type alignment) -> void* override final {
        return buffer_resource_.allocate(n_bytes, alignment);
    }
    void do_deallocate(void* ptr, size_type n_bytes, size_type alignment) override final {
        buffer_resource_.deallocate(ptr, n_bytes, alignment);
    }
    virtual auto do_extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* final {
        if constexpr (resource_can_extend) {
            return buffer_resource_.extend(ptr, old_bytes, new_bytes, alignment);
        } else {
            // If the resource cannot extend, we can only allocate a new block of memory
            return buffer_resource_.allocate(new_bytes, alignment);
        }
    }
    auto do_is_equal(resource_base const& other) const noexcept -> bool override final { return this == &other; }

    auto size() const noexcept -> size_type { return buffer_resource_.size(); }
    auto size_bytes() const noexcept -> size_type { return size() * sizeof(T); }
    auto capacity() const noexcept -> size_type { return CAPACITY_ELEMS; }
    auto capacity_bytes() const noexcept -> size_type { return CAPACITY_BYTES; }
    auto remaining_capacity() const noexcept -> size_type { return remaining_capacity_bytes() / sizeof(T); }
    auto remaining_capacity_bytes() const noexcept -> size_type { return buffer_resource_.remaining_capacity(); }
    auto empty() const noexcept -> bool { return buffer_resource_.empty(); }
  private:
    buffer_mmr<CAPACITY_BYTES> buffer_resource_;
};
}
