#pragma once

#include <memory>

#include "platform_def.hpp"

// Binary search tree
namespace ml {
namespace detail {
template <typename T>
class bst_node {};
}

template <typename T, typename Allocator = std::allocator<T>>
class bst {
  public:
    using value_type = T;
    using node_type = detail::bst_node<T>;
    using allocator_type = Allocator;

    // Capacity
    auto empty() const -> bool;
  private:
    NO_UNIQUE_ADDRESS Allocator alloc_;
    node_type* root_{nullptr};
};

#define METHOD_START                          \
    template <typename T, typename Allocator> \
    inline auto bst<T, Allocator>

METHOD_START::empty() const->bool {
    return true;
}

#undef METHOD_START
}

#include "platform_undef.hpp"
