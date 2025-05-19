#pragma once

#include <cstddef>
#include <utility>

#include "allocator_concepts.hpp"
#include "preprocessor/platform_def.hpp"
#include "preprocessor/noexcept_release_def.hpp"

namespace ml {
// Vector which can extend an existing allocation instead of allocating a new one
template <typename T, typename Allocator>
    requires (extendable_allocator<Allocator> && can_allocate_bytes<Allocator>)
class vector2 {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;

    // Constructors
    vector2() noexcept = default;
    template <typename U>
    vector2(U&& allocator)
        : allocator_{std::forward<U>(allocator)} {}
    ~vector2() {
        for (size_type i{0}; i < size_; ++i) {
            data_[i].~value_type();
        }
        allocator_.deallocate(data_, capacity_);
    }

    // Element access
    template <typename Self>
    auto at(this Self&& self, size_type index) {
        if (index >= self.size_) {
            throw std::out_of_range{"Index out of range"};
        }
        return std::forward<Self>(self).data_[index];
    }
    template <typename Self>
    auto back(this Self&& self) NOEXCEPT_RELEASE {
#ifdef DEBUG_ENABLED
        if (self.empty()) {
            throw std::out_of_range{"Vector is empty."};
        }
#endif
        auto const i{self.size_ - 1};
        return std::forward<Self>(self).data_[i];
    }
    template <typename Self>
    auto front(this Self&& self) NOEXCEPT_RELEASE {
#ifdef DEBUG_ENABLED
        if (self.empty()) {
            throw std::out_of_range{"Vector is empty."};
        }
#endif

        return std::forward<Self>(self).data_[0];
    }
    template <typename Self>
    auto operator[](this Self&& self, size_type index) NOEXCEPT_RELEASE {
#ifdef DEBUG_ENABLED
        if (index >= self.size_) {
            throw std::out_of_range{"Index out of range"};
        }
#endif
        return std::forward<Self>(self).data_[index];
    }

    // Capacity
    auto capacity() const noexcept -> size_type { return capacity_; }
    auto empty() const noexcept -> bool { return size_ == 0; }
    auto full() const noexcept -> bool { return size_ == capacity_; }
    auto size() const noexcept -> size_type { return size_; }

    // Modifiers
    template <typename... Args>
    void emplace_back(Args&&... args) {
        grow_if_full();
        new (data_ + size_) value_type{std::forward<Args>(args)...};
        ++size_;
    }
    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_].~value_type();
        }
    }
    template <typename U>
    void push_back(U&& new_elem) {
        grow_if_full();
        new (data_ + size_) value_type{std::forward<U>(new_elem)};
        ++size_;
    }
  private:
    // Capacity
    void grow() {
        auto const old_size{size_};
        auto const new_size{size_ ? size_ * 2 : 1};

        auto* current_data{data_};
        auto* new_data{allocator_.extend(current_data, old_size, new_size)};
        capacity_ = new_size;

        data_ = new_data;

        if (new_data != current_data) {
            // Move the objects to the new buffer
            for (std::size_t i{0}; i < old_size; ++i) {
                new (new_data + i) T{std::move(current_data[i])};
            }

            // Deallocate the old buffer
            allocator_.deallocate(current_data, old_size);
        }
    }
    void grow_if_full() {
        if (full()) {
            grow();
        }
    }

    NO_UNIQUE_ADDRESS Allocator allocator_;
    size_type size_{0};
    size_type capacity_{0};
    T* data_{nullptr};
};
}

#include "preprocessor/platform_undef.hpp"
#include "preprocessor/noexcept_release_undef.hpp"
