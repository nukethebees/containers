#pragma once

namespace ml {
template <typename T>
struct linked_vector_segment {
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    linked_vector_segment() = default;
    linked_vector_segment(linked_vector_segment* prev_, size_type capacity_, T* data_)
        : prev(prev_)
        , capacity(capacity_)
        , data(data_) {
    }

    linked_vector_segment(linked_vector_segment const&) = delete;
    linked_vector_segment(linked_vector_segment&&) = delete;

    linked_vector_segment& operator=(linked_vector_segment const&) = delete;
    linked_vector_segment& operator=(linked_vector_segment&&) = delete;

    ~linked_vector_segment() = default;

    linked_vector_segment* prev{nullptr};
    linked_vector_segment* next{nullptr};
    size_type size{0};
    size_type capacity{0};
    T* data{nullptr};
};




}
