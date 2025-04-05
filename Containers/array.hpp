#pragma once

#include <cstdint>

#include "span_iterator.hpp"

namespace ml {
template <typename T, std::size_t SIZE>
class array {
public:
    using value_t = T;
    using ptr_t = T *;
    using const_ptr_t = T const *;
    using size_t = std::size_t;
    using iter_t = span_iterator<T>;
private:
    T data_[SIZE];
public:
    array() = default;
    template <typename... U>
        requires (sizeof...(U) == SIZE)
    array(U&&... values) 
        : data_{std::forward<U>(values)...}
    {}

    auto size() const -> size_t {
        return SIZE;
    }
    template <typename Self>
    auto data(this Self && self) -> ptr_t {
        return std::forward<Self>(self).data_;
    }

    auto begin() -> iter_t {
        return iter_t(data_);
    }
    auto end() -> iter_t {
        return iter_t(data_ + SIZE);
    }

    // Operators
    auto operator[](size_t index) const -> value_t {
        return *(data_ + index);
    }
};
}