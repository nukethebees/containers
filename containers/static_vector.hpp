#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

#include "span_iterator.hpp"

namespace ml {
template <typename T, std::size_t CAPACITY>
class static_vector {
  private:
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
    auto at(this Self&& self, std::size_t idx);
    template <typename Self>
    auto back(this Self&& self);
    template <typename Self>
    auto data(this Self&& self);
    template <typename Self>
    auto front(this Self&& self);
    template <typename Self>
    auto operator[](this Self&& self, std::size_t idx);

    // Iterators
    auto begin(this static_vector& self) -> iterator;
    auto cbegin(this static_vector const& self) -> const_iterator;
    auto cend(this static_vector const& self) -> const_iterator;
    auto crbegin(this static_vector const& self) -> const_reverse_iterator;
    auto crend(this static_vector const& self) -> const_reverse_iterator;
    auto end(this static_vector& self) -> iterator;
    auto rbegin(this static_vector& self) -> reverse_iterator;
    auto rend(this static_vector& self) -> reverse_iterator;

    // Capacity
    constexpr auto capacity() const -> size_type;
    auto empty() const -> size_type;
    auto size() const -> size_type;

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

template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto static_vector<T, CAPACITY>::at(this Self&& self, std::size_t idx) {
    if (idx >= self.size_) {
        throw std::out_of_range{"static_vector: at: out of range"};
    }
    return std::forward<Self>(self).data_[idx].value;
}
template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto static_vector<T, CAPACITY>::back(this Self&& self) {
    return std::forward<Self>(self).data_[self.size_ - 1].value;
}
template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto static_vector<T, CAPACITY>::data(this Self&& self) {
    return std::forward<Self>(self).data_.data();
}
template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto static_vector<T, CAPACITY>::front(this Self&& self) {
    return std::forward<Self>(self).data_[0].value;
}
template <typename T, std::size_t CAPACITY>
template <typename Self>
inline auto static_vector<T, CAPACITY>::operator[](this Self&& self, std::size_t idx) {
    return std::forward<Self>(self).data_[idx].value;
}

template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::begin(this static_vector& self) -> iterator {
    return iterator(reinterpret_cast<T*>(self.data()));
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::cbegin(this static_vector const& self) -> const_iterator {
    return const_iterator(reinterpret_cast<T const*>(self.data()));
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::cend(this static_vector const& self) -> const_iterator {
    return const_iterator(reinterpret_cast<T const*>(self.data()) + self.size_);
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::crbegin(this static_vector const& self) -> const_reverse_iterator {
    return const_reverse_iterator(self.cend());
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::crend(this static_vector const& self) -> const_reverse_iterator {
    return const_reverse_iterator(self.cbegin());
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::end(this static_vector& self) -> iterator {
    return iterator(reinterpret_cast<T*>(self.data()) + self.size_);
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::rbegin(this static_vector& self) -> reverse_iterator {
    return reverse_iterator(self.end());
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::rend(this static_vector& self) -> reverse_iterator {
    return reverse_iterator(self.begin());
}

template <typename T, std::size_t CAPACITY>
inline constexpr auto static_vector<T, CAPACITY>::capacity() const -> size_type {
    return CAPACITY;
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::empty() const -> size_type {
    return size_ == 0;
}
template <typename T, std::size_t CAPACITY>
inline auto static_vector<T, CAPACITY>::size() const -> size_type {
    return size_;
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
