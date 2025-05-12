#pragma once

#include <cstddef>

#include "linked_vector_segment.hpp"
#include "linked_vector_iterator.hpp"
#include "allocator.hpp"

#include "platform_def.hpp"

namespace ml {
/*
A hybrid data structure of a linked list and a vector.
It primarily acts as a vector but new allocations are made in a linked list fashion
instead of copying the data to a new location.

Vector elements: XXXXXXXXXXXXXXXXXXXXXXXXXXXXX
List elements:   X<->X<->X<->X<->X<->X
Hybrid elements: X<->XX<->XXXX<->XXXXXXXX<->XXXXXXXXXXXXXXXX

The initial capacity can be set by explicitly reserving the memory.
The growth factor is 2x.
*/
template <typename T, typename Allocator = ml::allocator<std::byte>>
    requires can_allocate_bytes<Allocator>
class linked_vector {
  public:
    using value_type = T;
    using segment_type = linked_vector_segment<value_type>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using allocator_type = Allocator;
    using iterator = linked_vector_iterator<segment_type>;
    using const_iterator = linked_vector_iterator<segment_type const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr auto data_alignment{alignof(value_type)};
    static constexpr auto segment_alignment{alignof(segment_type)};

    linked_vector() noexcept = default;
    ~linked_vector();

    // Element access
    auto data() -> pointer;
    auto data() const -> const_pointer;

    // Iterators
    auto begin() -> iterator;
    auto begin() const -> const_iterator;
    auto cbegin() const -> const_iterator;
    auto cend() const -> const_iterator;
    auto crbegin() const -> const_reverse_iterator;
    auto crend() const -> const_reverse_iterator;
    auto end() -> iterator;
    auto end() const -> const_iterator;
    auto rbegin() -> reverse_iterator;
    auto rbegin() const -> const_reverse_iterator;
    auto rend() -> reverse_iterator;
    auto rend() const -> const_reverse_iterator;

    // Capacity
    auto capacity() const -> size_type;
    auto empty() const -> bool;
    auto reserve(size_type n_elems) -> void;
    auto size() const -> size_type;

    // Modifiers
    void clear();
    template <typename... Args>
    void emplace_back(Args&&... args);
    template <typename U>
    void push_back(U&&);
  private:
    // Capacity
    void construct_segment(size_type size);
    void destroy_segment(segment_type* segment);
    // Modifiers
    void destroy_segment_elements(pointer ptr, size_type n);

    NO_UNIQUE_ADDRESS Allocator alloc_;
    size_type size_{0};
    size_type capacity_{0};
    segment_type* head_{nullptr};
    segment_type* tail_{nullptr};
};

template <typename T, typename Allocator>
    requires can_allocate_bytes<Allocator>
inline linked_vector<T, Allocator>::~linked_vector() {
    auto* segment{head_};
    while (segment) {
        auto* next_segment{segment->next};
        destroy_segment(segment);
        segment = next_segment;
    }
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

#define METHOD_START(...)                      \
    template <typename T, typename Allocator>  \
        requires can_allocate_bytes<Allocator> \
    __VA_OPT__(__VA_ARGS__)                    \
    inline auto linked_vector<T, Allocator>

// Element access
METHOD_START()::data()->pointer {
    return head_ ? head_->data : nullptr;
}
METHOD_START()::data() const->const_pointer {
    return head_ ? head_->data : nullptr;
}

// Iterators
METHOD_START()::begin()->iterator {
    return iterator(head_);
}
METHOD_START()::begin() const->const_iterator {
    return cbegin();
}
METHOD_START()::cbegin() const->const_iterator {
    return const_iterator(head_);
}
METHOD_START()::cend() const->const_iterator {
    return const_iterator(tail_, nullptr, 0);
}
METHOD_START()::crbegin() const->const_reverse_iterator {
    return const_reverse_iterator(cend());
}
METHOD_START()::crend() const->const_reverse_iterator {
    return const_reverse_iterator(cbegin());
}
METHOD_START()::end()->iterator {
    return iterator(tail_, nullptr, 0);
}
METHOD_START()::end() const->const_iterator {
    return cend();
}
METHOD_START()::rbegin()->reverse_iterator {
    return reverse_iterator(end());
}
METHOD_START()::rbegin() const->const_reverse_iterator {
    return crbegin();
}
METHOD_START()::rend()->reverse_iterator {
    return reverse_iterator(begin());
}
METHOD_START()::rend() const->const_reverse_iterator {
    return crend();
}

// Capacity
METHOD_START()::capacity() const->size_type {
    return capacity_;
}
METHOD_START()::empty() const->bool {
    return size_ == 0;
}
METHOD_START()::reserve(size_type n_elems)->void {
    if (n_elems > capacity_) {
        auto const elements_needed{n_elems - capacity_};

        construct_segment(elements_needed);
        capacity_ = n_elems;
    }
}
METHOD_START()::size() const->size_type {
    return size_;
}

// Modifiers
METHOD_START()::clear()->void {
    auto* segment{head_};
    while (segment) {
        destroy_segment_elements(segment->data, segment->size);
        segment->size = 0;
        segment = segment->next;
    }

    size_ = 0;
}
METHOD_START(template <typename... Args>)::emplace_back(Args&&... args)->void {
    if (size_ == capacity_) {
        auto const new_capacity{capacity_ ? capacity_ * 2 : 1};
        reserve(new_capacity);
    }

    auto* segment{tail_};
    new (&segment->data[segment->size]) value_type(std::forward<Args>(args)...);

    ++segment->size;
    ++size_;
}
METHOD_START(template <typename U>)::push_back(U&& value)->void {
    if (size_ == capacity_) {
        auto const new_capacity{capacity_ ? capacity_ * 2 : 1};
        reserve(new_capacity);
    }

    auto* segment{tail_};
    new (&segment->data[segment->size]) value_type(std::forward<U>(value));

    ++segment->size;
    ++size_;
}

// Private
// Capacity
METHOD_START()::construct_segment(size_type n_elems)->void {
    static constexpr auto segment_bytes{sizeof(segment_type)};
    auto data_bytes{sizeof(value_type) * n_elems};

    // Allocate space for the segment and the data
    void* segment_ptr{alloc_.allocate_bytes(segment_bytes, segment_alignment)};
    void* data_ptr{alloc_.allocate_bytes(data_bytes, data_alignment)};

    // Construct the segment
    auto* segment{new (segment_ptr) segment_type(tail_, n_elems, reinterpret_cast<pointer>(data_ptr))};

    if (tail_) {
        tail_->next = segment;
    } else {
        head_ = segment;
    }
    tail_ = segment;
}
METHOD_START()::destroy_segment(segment_type* segment)->void {
    auto* const seg_data{segment->data};
    auto const seg_size{segment->size};
    auto const seg_capacity{segment->capacity};

    destroy_segment_elements(seg_data, seg_size);

    if (segment) {
        auto const data_bytes{sizeof(value_type) * seg_capacity};

        alloc_.deallocate_bytes(reinterpret_cast<void*>(seg_data), data_bytes, data_alignment);
        alloc_.deallocate_bytes(reinterpret_cast<void*>(segment), sizeof(segment_type), segment_alignment);
    }
}
// Modifiers
METHOD_START()::destroy_segment_elements(pointer ptr, size_type n)->void {
    for (size_type i{0}; i < n; ++i) {
        ptr[i].~value_type();
    }
}

#undef METHOD_START

}

#include "platform_undef.hpp"
