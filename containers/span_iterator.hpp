#pragma once

namespace ml {
template <typename T>
class span_iterator {
  private:
    T* ptr{nullptr};
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using element_type = T;
    using iterator_category = std::contiguous_iterator_tag;

    // Needed to satisfy the default constructible requirement
    span_iterator() = default;
    explicit span_iterator(T* ptr)
        : ptr(ptr) {}

    // Dereferencing
    // For some reason, a const iterator needs to return T& to meet the iter requirements
    auto operator*() const -> reference { return *ptr; }
    auto operator*() -> reference { return *ptr; }
    auto operator->() const -> T* { return ptr; }
    auto operator[](difference_type n) const -> reference { return *(ptr + n); }

    // Increment / decrement
    auto operator++() -> span_iterator& {
        ++ptr;
        return *this;
    }
    auto operator++(int) -> span_iterator {
        auto temp{*this};
        ++(*this);
        return temp;
    }

    auto operator--() -> span_iterator& {
        --ptr;
        return *this;
    }
    auto operator--(int) -> span_iterator {
        auto temp{*this};
        --(*this);
        return temp;
    }

    // Math operators
    auto operator+(difference_type n) const -> span_iterator { return span_iterator(ptr + n); }
    friend auto operator+(difference_type lhs, span_iterator const& rhs) -> span_iterator {
        return span_iterator(lhs + rhs.ptr);
    }
    auto operator-(difference_type n) const -> span_iterator { return span_iterator(ptr - n); }
    friend auto operator-(difference_type lhs, span_iterator const& rhs) -> span_iterator {
        return span_iterator(lhs - rhs.ptr);
    }
    auto operator-(span_iterator const& n) const -> difference_type { return difference_type{ptr - n.ptr}; }
    auto operator+=(difference_type n) -> span_iterator& {
        ptr += n;
        return *this;
    }
    auto operator-=(difference_type n) -> span_iterator& {
        ptr -= n;
        return *this;
    }

    // Comparison
    auto operator==(span_iterator const& other) const -> bool { return ptr == other.ptr; }
    auto operator!=(span_iterator const& other) const -> bool { return ptr != other.ptr; }
    auto operator<=>(span_iterator const&) const = default;
};

static_assert(std::input_or_output_iterator<ml::span_iterator<int>>);
static_assert(std::input_iterator<ml::span_iterator<int>>);
static_assert(std::incrementable<ml::span_iterator<int>>);
static_assert(std::forward_iterator<ml::span_iterator<int>>);
static_assert(std::bidirectional_iterator<ml::span_iterator<int>>);
static_assert(std::random_access_iterator<ml::span_iterator<int>>);
static_assert(std::contiguous_iterator<ml::span_iterator<int>>);
}
