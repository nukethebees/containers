#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "contiguous_container_mixins.hpp"
#include "iterator_boilerplate.hpp"
#include "span_iterator.hpp"

#include "preprocessor/platform_def.hpp"

namespace ml {
template <typename T, typename Allocator = std::allocator<T>>
class vector
    : public ContiguousIteratorMethods
    , public ContiguousContainerCommonMethods
    , public ContiguousContainerCommonCapacityMethods {
    friend struct ContiguousContainerCommonMethods;
    friend struct ContiguousContainerCommonCapacityMethods;
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

    // Ctor
    vector() noexcept
        : alloc{} {}
    vector(Allocator const& alloc)
        : alloc{alloc} {}
    // Copy ctor
    vector(vector const& other, Allocator const& alloc)
        : alloc{alloc} {
        reserve(other.capacity_);
        for (auto const& item : other) {
            push_back(item);
        }
    }
    vector(vector const& other)
        : vector(other, other.alloc) {}
    // Move ctor
    vector(vector&& other)
        : data_{other.data_}
        , size_{other.size_}
        , capacity_{other.capacity_}
        , alloc{std::move(other.alloc)} {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // Copy assignment
    auto& operator=(vector const& other) {
        clear();
        for (auto const& item : other) {
            push_back(item);
        }
    }
    // Move assignment
    auto& operator=(vector&& other) {
        clear();
        alloc = std::move(other.alloc);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~vector() {
        for (auto i{0uz}; i < size_; ++i) {
            data_[i].~T();
        }
        alloc.deallocate(data_, capacity_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    // Capacity
    auto capacity(this vector const& self) -> std::size_t { return self.capacity_; }

    auto max_size() const -> std::size_t { return std::numeric_limits<std::size_t>::max(); }

    void reserve(this vector& self, std::size_t new_capacity) {
        if (new_capacity <= self.capacity_) {
            return;
        }

        auto* new_data{self.alloc.allocate(new_capacity)};

        for (auto i{0uz}; i < self.size_; i++) {
            new (new_data + i) T{std::move(self.data_[i])};
        }

        self.alloc.deallocate(self.data_, self.capacity_);
        self.data_ = new_data;
        self.capacity_ = new_capacity;
    }

    void shrink_to_fit() { return; }

    // Modifiers
    void clear(this vector& self) {
        for (auto i{0uz}; i < self.size_; ++i) {
            self.data_[i].~T();
        }
        self.size_ = 0;
    }

    template <typename... Args>
    void emplace_back(this vector& self, Args&&... args) {
        auto const new_size{self.size_ + 1};

        if (new_size >= self.capacity_) {
            self.grow();
        }

        new (self.data_ + self.size_) T{std::forward<Args>(args)...};
        ++self.size_;
    }

    template <typename U>
        requires std::constructible_from<T, U>
    void push_back(this vector& self, U&& value) {
        auto const new_size{self.size_ + 1};
        if (new_size >= self.capacity_) {
            self.grow();
        }
        new (self.data_ + self.size_) T{std::forward<U>(value)};
        ++self.size_;
    }
  private:
    T* data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};
    NO_UNIQUE_ADDRESS Allocator alloc;

    void grow(this vector& self) { self.reserve(self.capacity_ ? self.capacity_ * 2uz : 1uz); }
};

}

#include "preprocessor/platform_undef.hpp"
