#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "platform_def.hpp"

// Binary search tree
namespace ml {
namespace detail {
template <typename T>
class bst_node {};
}

template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
class bst {
  public:
    using value_type = T;
    using size_type = std::size_t;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using node_type = detail::bst_node<T>;

    // Capacity
    auto empty() const -> bool;
    auto size() const -> bool;
  private:
    inline static Compare compare{};
    NO_UNIQUE_ADDRESS Allocator alloc_;
    node_type* root_{nullptr};
    size_type size_{0};
};

#define METHOD_START                                            \
    template <typename T, typename Compare, typename Allocator> \
    inline auto bst<T, Compare, Allocator>

METHOD_START::empty() const->bool {
    return size_ == 0;
}
METHOD_START::size() const->bool {
    return size_;
}

#undef METHOD_START
}

#include "platform_undef.hpp"
