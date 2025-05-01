#pragma once

#include <array>
#include <cstddef>

#include "span_iterator.hpp"

namespace ml {
template <typename T, std::size_t CAPACITY>
class static_vector {
  private:
    union array_elem_t {
        T value;

        ~array_elem_t() {}
    };
  public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;
    using iterator = span_iterator<T>;
    using const_iterator = span_iterator<T const>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static_vector() = default;
    ~static_vector();

    constexpr auto capacity() const -> size_type;
    auto size() const -> size_type;
  private:
    std::array<array_elem_t, CAPACITY> elements_;
    size_type size_{0};
};

template <typename T, std::size_t CAPACITY>
inline static_vector<T, CAPACITY>::~static_vector() {
    for (size_type i{0}; i < size_; ++i) {
        elements_[i].value.~T();
    }
}

template <typename T, std::size_t CAPACITY>
inline constexpr auto static_vector<T, CAPACITY>::capacity() const -> size_type {
    return CAPACITY;
}

template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::size() const -> size_type {
    return size_;
}
}
