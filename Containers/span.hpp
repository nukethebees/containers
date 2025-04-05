#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace ml {
template <typename T>
concept has_data_and_size = requires (T t) {
    { t.size() } -> std::same_as<std::size_t>;
    requires std::is_pointer_v<decltype(t.data())>;
};

template <typename T>
class span_iterator {
private:
    T * ptr{nullptr};
public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using element_type = T;
    using iterator_category = std::contiguous_iterator_tag;

    // Needed to satisfy the default constructible requirement
    span_iterator() = default;
    explicit span_iterator(T * ptr) : ptr(ptr) {}

    // Dereferencing
    // For some reason, a const iterator needs to return T& to meet the iter requirements
    auto operator*() const -> reference {
        return *ptr;
    }
    auto operator*() -> reference {
        return *ptr;
    }
    auto operator->() const -> T * {
        return ptr;
    }
    auto operator[](difference_type n) const -> reference {
        return *(ptr + n);
    }

    // Increment / decrement
    auto operator++() -> span_iterator & {
        ++ptr;
        return *this;
    }
    auto operator++(int) -> span_iterator {
        auto temp{*this};
        ++(*this);
        return temp;
    }

    auto operator--() -> span_iterator & {
        --ptr;
        return *this;
    }
    auto operator--(int) -> span_iterator {
        auto temp{*this};
        --(*this);
        return temp;
    }

    // Math operators
    auto operator+(difference_type n) const -> span_iterator {
        return span_iterator(ptr + n);
    }
    friend auto operator+(difference_type lhs, const span_iterator & rhs) -> span_iterator {
        return span_iterator(lhs + rhs.ptr);
    }
    auto operator-(difference_type n) const -> span_iterator {
        return span_iterator(ptr - n);
    }
    friend auto operator-(difference_type lhs, const span_iterator & rhs) -> span_iterator {
        return span_iterator(lhs - rhs.ptr);
    }
    auto operator-(span_iterator const & n) const -> difference_type {
        return difference_type{ptr - n.ptr};
    }
    auto operator+=(difference_type n) -> span_iterator & {
        ptr += n;
        return *this;
    }
    auto operator-=(difference_type n) -> span_iterator & {
        ptr -= n;
        return *this;
    }

    // Comparison
    auto operator==(const span_iterator & other) const -> bool {
        return ptr == other.ptr;
    }
    auto operator!=(const span_iterator & other) const -> bool {
        return ptr != other.ptr;
    }
    auto operator<=>(span_iterator const &) const = default;
};

template <typename T>
class span {
public:
    using value_t = T;
    using ptr_t = T *;
    using const_ptr_t = T const *;
    using size_t = std::size_t;
    using iter_t = span_iterator<T>;

private:
    ptr_t data_;
    size_t size_;
public:
    span() = delete;
    span(ptr_t ptr, size_t size) noexcept
        : data_{ptr}
        , size_{size} {}
    template <typename U>
        requires has_data_and_size<U>
    span(U & container) noexcept
        : data_{container.data()}
        , size_{container.size()} {}

    auto size() const -> size_t {
        return size_;
    }
    template <typename Self>
    auto data(this Self && self) -> ptr_t {
        return std::forward<Self>(self).data_;
    }

    auto begin() -> iter_t {
        return iter_t(data_);
    }
    auto end() -> iter_t {
        return iter_t(data_ + size_);
    }

    // Operators
    auto operator[](size_t index) const -> value_t {
        return *(data_ + index);
    }
};

template <typename T>
    requires has_data_and_size<T>
span(T & iter)->span<std::remove_cvref_t<decltype(*iter.data())>>;
}

static_assert(std::input_or_output_iterator<ml::span_iterator<int>>);
static_assert(std::input_iterator<ml::span_iterator<int>>);
static_assert(std::incrementable<ml::span_iterator<int>>);
static_assert(std::forward_iterator<ml::span_iterator<int>>);
static_assert(std::bidirectional_iterator<ml::span_iterator<int>>);
static_assert(std::random_access_iterator<ml::span_iterator<int>>);
static_assert(std::contiguous_iterator<ml::span_iterator<int>>);