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
class span {
public:
    using value_t = T;
    using ptr_t = T *;
    using const_ptr_t = T const *;
    using size_t = std::size_t;

    class Iterator {
    private:
        T * ptr{nullptr};
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        // Needed to satisfy the default constructible requirement
        Iterator() = default;
        explicit Iterator(T * ptr) : ptr(ptr) {}

        // Dereferencing
        // For some reason, a const iterator needs to return T& to meet the iter requirements
        auto operator*() const -> reference {
            return *ptr;
        }
        auto operator*() -> reference {
            return *ptr;
        }
        auto operator->() -> pointer {
            return ptr;
        }
        auto operator->() const -> T const * {
            return ptr;
        }
        auto operator[](difference_type n) const -> reference {
            return *(ptr + n);
        }

        // Increment / decrement
        auto operator++() -> Iterator & {
            ++ptr;
            return *this;
        }
        auto operator++(int) -> Iterator {
            Iterator temp{*this};
            ++(*this);
            return temp;
        }

        auto operator--() -> Iterator & {
            --ptr;
            return *this;
        }
        auto operator--(int) -> Iterator {
            Iterator temp{*this};
            --(*this);
            return temp;
        }

        // Math operators
        auto operator+(difference_type n) const -> Iterator {
            return Iterator(ptr + n);
        }
        auto operator-(difference_type n) const -> Iterator {
            return Iterator(ptr - n);
        }
        auto operator-(Iterator const& n) const -> difference_type {
            return difference_type{ptr - n.ptr};
        }
        auto operator+=(difference_type n) -> Iterator & {
            ptr += n;
            return *this;
        }
        auto operator-=(difference_type n) -> Iterator & {
            ptr -= n;
            return *this;
        }

        // Comparison
        auto operator==(const Iterator & other) const -> bool {
            return ptr == other.ptr;
        }
        auto operator!=(const Iterator & other) const -> bool {
            return ptr != other.ptr;
        }
        auto operator<=>(Iterator const&) const = default;
    };
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

    Iterator begin() {
        return Iterator(data_);
    }
    Iterator end() {
        return Iterator(data_ + size_);
    }
};

template <typename T>
    requires has_data_and_size<T>
span(T & iter)->span<std::remove_cvref_t<decltype(*iter.data())>>;
}

static_assert(std::input_or_output_iterator<ml::span<int>::Iterator>);
static_assert(std::input_iterator<ml::span<int>::Iterator>);
static_assert(std::incrementable<ml::span<int>::Iterator>);
static_assert(std::forward_iterator<ml::span<int>::Iterator>);
static_assert(std::bidirectional_iterator<ml::span<int>::Iterator>);
//static_assert(std::random_access_iterator<ml::span<int>::Iterator>);