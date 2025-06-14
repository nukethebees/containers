#pragma once

#include <cstddef>
#include <cstdint>

#include "contiguous_container_common_methods.hpp"
#include "iterator_boilerplate.hpp"
#include "span_iterator.hpp"

namespace ml {
template <typename T, std::size_t N>
class array
    : public ContiguousIteratorMethods
    , public ContiguousContainerCommonMethods {
    friend struct ContiguousContainerCommonMethods;
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
  private:
    T data_[N];
  public:
    constexpr array() = default;
    template <typename... U>
        requires (sizeof...(U) == N)
    constexpr array(U&&... values)
        : data_{std::forward<U>(values)...} {}

    // Capacity
    constexpr auto empty() const -> bool { return N == 0; }
    constexpr auto size_bytes() const -> std::size_t { return N * sizeof(value_type); }
    constexpr auto size() const -> size_type { return N; }
    constexpr auto max_size() const -> size_type { return N; }
};

template <typename T>
class array<T, 0> {
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

    constexpr array() = default;

    // Element access
    constexpr auto at(size_type) -> reference { throw std::out_of_range{"Index out of range"}; }
    constexpr auto operator[](size_type) const -> value_type {
        throw std::out_of_range{"Index out of range"};
    }
    constexpr auto front() -> reference { throw std::out_of_range{"Array is empty"}; }
    constexpr auto front() const -> const_reference { throw std::out_of_range{"Array is empty"}; }
    constexpr auto back() -> reference { throw std::out_of_range{"Array is empty"}; }
    constexpr auto back() const -> const_reference { throw std::out_of_range{"Array is empty"}; }
    template <typename Self>
    constexpr auto data(this Self&&) -> pointer {
        return nullptr;
    }

    // Iterators
    auto begin() -> iterator { return iterator(nullptr); }
    auto cbegin() const -> const_iterator { return const_iterator(nullptr); }
    auto end() -> iterator { return iterator(nullptr); }
    auto cend() const -> const_iterator { return const_iterator(nullptr); }

    // Capacity
    constexpr auto empty() const -> bool { return true; }
    constexpr auto size_bytes() const -> std::size_t { return 0; }
    constexpr auto size() const -> size_type { return 0; }
    constexpr auto max_size() const -> size_type { return 0; }
};
}
