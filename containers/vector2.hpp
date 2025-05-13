#pragma once

#include <cstddef>
#include <utility>

#include "allocator_concepts.hpp"
#include "platform_def.hpp"

namespace ml {
// Vector which can extend an existing allocation instead of allocating a new one
template <typename T, typename Allocator>
    requires extendable_allocator<Allocator> && can_allocate_bytes<Allocator>
class vector2 {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;

    vector2() = default;
    template <typename U>
    vector2(U&& allocator)
        : allocator_{std::forward<U>(allocator)} {}

    // Capacity
    auto capacity() const -> size_type { return capacity_; }
    auto empty() const -> bool { return size_ == 0; }
    auto size() const -> size_type { return size_; }
  private:
    NO_UNIQUE_ADDRESS Allocator allocator_;
    size_type size_{0};
    size_type capacity_{0};
    T* data_{nullptr};
};
}

#include "platform_undef.hpp"
