#pragma once

#include <cstddef>
#include <utility>

#include "allocator_concepts.hpp"
#include "platform_def.hpp"

namespace ml {
// Vector which can extend an existing allocation instead of allocating a new one
template <typename T, typename Allocator>
    requires (extendable_allocator<Allocator> && can_allocate_bytes<Allocator>)
class vector2 {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;

    vector2() = default;
    template <typename U>
    vector2(U&& allocator)
        : allocator_{std::forward<U>(allocator)} {}

    // Capacity
    auto capacity() const -> size_type { return capacity_; }
    auto empty() const -> bool { return size_ == 0; }
    auto full() const -> bool { return size_ == capacity_; }
    auto size() const -> size_type { return size_; }

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

#include "platform_undef.hpp"
