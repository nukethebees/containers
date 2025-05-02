#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "platform_def.hpp"

namespace ml {
// Red-black tree set
template <typename Key, typename Compare = std::less<Key>, typename Allocator = std::allocator<Key>>
class rbset {
  public:
    using key_type = Key;
    using value_type = Key;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    rbset() = default;

    // Capacity
    auto empty() const -> bool;
    auto size() const -> size_type;
  private:
    static inline auto compare{Compare{}};
    size_type size_{0};
};

#define METHOD_START                                              \
    template <typename Key, typename Compare, typename Allocator> \
    inline auto rbset<Key, Compare, Allocator>

METHOD_START::empty() const->bool {
    return size_ == 0;
}
METHOD_START::size() const->size_type {
    return size_;
}
}

#undef METHOD_START
#include "platform_undef.hpp"
