#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

#include "contiguous_container_mixins.hpp"
#include "iterator_boilerplate.hpp"
#include "span_iterator.hpp"

namespace ml {
template <typename T, std::size_t CAPACITY>
class static_vector
    : public ContiguousIteratorMethods
    , public ContiguousContainerCommonMethods
    , public ContiguousContainerCommonCapacityMethods {
  private:
    friend struct ContiguousContainerCommonMethods;
    friend struct ContiguousContainerCommonCapacityMethods;

    union array_elem_t {
        T value;

        array_elem_t() {}
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

    // Access
    template <typename Self>
    auto* data(this Self&& self);

    // Capacity
    constexpr auto capacity() const -> size_type;

    // Modification
    void clear();
    template <typename... Args>
    void emplace_back(Args&&... args);
    void pop_back();
    template <typename U>
    void push_back(U&& value);
  private:
    std::array<array_elem_t, CAPACITY> data_;
    size_type size_{0};
};

template <typename T, std::size_t CAPACITY>
inline static_vector<T, CAPACITY>::~static_vector() {
    for (size_type i{0}; i < size_; ++i) {
        data_[i].value.~T();
    }
}

// Access
template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto* static_vector<T, CAPACITY>::data(this Self&& self) {
    using ptr_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, T const*, T*>;
    return reinterpret_cast<ptr_t>(std::forward<Self>(self).data_.data());
}

// Capacity
template <typename T, std::size_t CAPACITY>
inline constexpr auto static_vector<T, CAPACITY>::capacity() const -> size_type {
    return CAPACITY;
}

template <typename T, std::size_t CAPACITY>
inline void static_vector<T, CAPACITY>::clear() {
    for (size_type i{0}; i < size_; ++i) {
        data_[i].value.~T();
    }
    size_ = 0;
}
template <typename T, std::size_t CAPACITY>
template <typename... Args>
inline void static_vector<T, CAPACITY>::emplace_back(Args&&... args) {
    if (size_ >= CAPACITY) {
        throw std::out_of_range{"static_vector: push_back: out of range"};
    }
    new (&data_[size_].value) T{std::forward<Args>(args)...};
    ++size_;
}
template <typename T, std::size_t CAPACITY>
inline void static_vector<T, CAPACITY>::pop_back() {
    if (size_ == 0) {
        throw std::out_of_range{"static_vector: pop_back: out of range"};
    }
    --size_;
    data_[size_].value.~T();
}
template <typename T, std::size_t CAPACITY>
template <typename U>
void static_vector<T, CAPACITY>::push_back(U&& value) {
    if (size_ >= CAPACITY) {
        throw std::out_of_range{"static_vector: push_back: out of range"};
    }
    new (&data_[size_].value) T{std::forward<U>(value)};
    ++size_;
}
}
