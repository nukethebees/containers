#pragma once

#include "platform.hpp"

namespace ml {
// Doubly-linked list
template <typename T, typename Allocator = std::allocator<T>>
class dlist {
  public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;

    dlist() noexcept = default;
  private:
    struct Node {};

    Node* head_{nullptr};
    Node* tail_{nullptr};
    size_type size_{0};
    NO_UNIQUE_ADDRESS Allocator alloc_{};
};
}
