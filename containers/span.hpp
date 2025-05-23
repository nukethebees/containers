#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>

#include "span_iterator.hpp"

namespace ml {
template <typename T>
concept has_data_and_size = requires(T t) {
    { t.size() } -> std::same_as<std::size_t>;
    requires std::is_pointer_v<decltype(t.data())>;
};

template <typename T>
class span {
  public:
    using element_type = T;
    using value_type = std::remove_cvref_t<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    using iterator = span_iterator<T>;
    using const_iterator = span_iterator<T const>;
  private:
    pointer data_;
    size_type size_;
  public:
    span() = delete;
    span(pointer ptr, size_type size) noexcept
        : data_{ptr}
        , size_{size} {}
    template <typename U>
        requires has_data_and_size<U>
    span(U& container) noexcept
        : data_{container.data()}
        , size_{container.size()} {}

    // Iterators
    auto begin() -> iterator { return iterator(data_); }
    auto cbegin() const -> const_iterator { return const_iterator(data_); }
    auto end() -> iterator { return iterator(data_ + size_); }
    auto cend() const -> const_iterator { return const_iterator(data_ + size_); }

    // Element access
    auto front() const -> reference { return *data_; }
    auto back() const -> reference { return *(data_ + size_ - 1); }
    auto at(size_type n) const -> reference {
        if (n >= size_) {
            throw std::out_of_range{"span::at(): index out of range"};
        }
        return *(data_ + n);
    }
    auto operator[](size_type index) const -> reference { return *(data_ + index); }
    template <typename Self>
    auto data(this Self&& self) -> pointer {
        return std::forward<Self>(self).data_;
    }

    // Observers
    auto size() const -> size_type { return size_; }
    auto size_bytes() const -> std::size_t { return size_ * sizeof(value_type); }
    auto empty() const -> bool { return size_ == 0; }
};

template <typename T>
    requires has_data_and_size<T>
span(T& iter) -> span<std::remove_cvref_t<decltype(*iter.data())>>;
}
