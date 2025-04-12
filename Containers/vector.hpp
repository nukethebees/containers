#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "span_iterator.hpp"

namespace ml {
template <typename T>
class vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T &;
    using const_reference = T const &;
    using pointer = T *;
    using const_pointer = T const *;
    using iterator = span_iterator<T>;
    using const_iterator = span_iterator<T const>;
private:
    T * data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};
    static inline std::allocator<T> alloc{};

    void grow(this vector & self) {
        self.reserve(self.capacity_ ? self.capacity_ * 2uz : 1uz);
    }
public:
    vector() noexcept = default;
    ~vector() {
        for (auto i{0uz}; i < size_; ++i) {
            data_[i].~T();
        }
        alloc.deallocate(data_, capacity_);
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    // Element access
    template <typename Self>
    auto && at(this Self && self, std::size_t idx) {
        if (idx >= self.size_) {
            throw std::out_of_range{"Index out of range"};
        }
        return std::forward<Self>(self).data_[idx];
    }
    template <typename Self>
    auto && operator[](this Self && self, std::size_t idx) {
        return std::forward<Self>(self).data_[idx];
    }
    template <typename Self>
    auto && front(this Self && self) {
        return std::forward<Self>(self).data_[0];
    }
    template <typename Self>
    auto && back(this Self && self) {
        return std::forward<Self>(self).data_[self.size_ - 1];
    }
    template <typename Self>
    auto * data(this Self && self) {
        return std::forward<Self>(self).data_;
    }

    // Iterators
    auto begin(this vector & self) -> iterator {
        return iterator(self.data_);
    }
    auto cbegin(this vector const & self) -> const_iterator {
        return const_iterator(self.data_);
    }
    auto end(this vector & self) -> iterator {
        return iterator(self.data_ + self.size_);
    }
    auto cend(this vector const & self) -> const_iterator {
        return const_iterator(self.data_ + self.size_);
    }

    // Capacity
    auto empty(this vector const & self) -> bool {
        return self.size_ == 0;
    }
    auto size(this vector const & self) -> std::size_t {
        return self.size_;
    }
    auto max_size(this vector const & self) -> std::size_t {
        return std::numeric_limits<std::size_t>::max();
    }
    void reserve(this vector & self, std::size_t new_capacity) {
        if (new_capacity <= self.capacity_) {
            return;
        }

        auto * new_data = alloc.allocate(new_capacity);

        for (auto i{0uz}; i < self.size_; i++) {
            new (new_data + i) T{std::move(self.data_[i])};
        }

        alloc.deallocate(self.data_, self.capacity_);
        self.data_ = new_data;
        self.capacity_ = new_capacity;
    }
    auto capacity(this vector const & self) -> std::size_t {
        return self.capacity_;
    }
    void shrink_to_fit(this vector & self) {
        return;
    }

    // Modifieers
    void clear(this vector & self) {
        for (auto i{0uz}; i < self.size_; ++i) {
            self.data_[i].~T();
        }
        self.size_ = 0;
    }
    template <typename... Args>
    void emplace_back(this vector & self, Args&&... args) {
        auto const new_size{self.size_ + 1};

        if (new_size >= self.capacity_) {
            self.grow();
        }

        new (self.data_ + self.size_) T{std::forward<Args>(args)...};
        ++self.size_;
    }

};
}