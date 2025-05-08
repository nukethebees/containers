#pragma once

#include <cstddef>

#include "platform_def.hpp"

namespace ml {
/*
A hybrid data structure of a linked list and a vector.
It primarily acts as a vector but new allocations are made in a linked list fashion
instead of copying the data to a new location.

Vector elements: XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
List elements:   X->X->X->X->X->X
Hybrid elements: X->XX->XXXX->XXXXXXXX->XXXXXXXXXXXXXXXX

The initial capacity can be set by explicitly reserving the memory.
The growth factor is 2x.
*/
template <typename T, typename Allocator = std::allocator<T>>
class linked_vector {
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using allocator_type = Allocator;

    linked_vector() noexcept = default;

    // Capacity
    auto capacity() const -> size_type;
    auto empty() const -> bool;
    auto size() const -> size_type;
  private:
    NO_UNIQUE_ADDRESS Allocator alloc;
    std::size_t size_{0};
    std::size_t capacity_{0};
};

#define METHOD_START(...)                     \
    template <typename T, typename Allocator> \
    __VA_OPT__(__VA_ARGS__)                   \
    inline auto linked_vector<T, Allocator>

METHOD_START()::capacity() const->size_type {
    return capacity_;
}
METHOD_START()::empty() const->bool {
    return size_ == 0;
}
METHOD_START()::size() const->size_type {
    return size_;
}

#undef METHOD_START

}

#include "platform_undef.hpp"
