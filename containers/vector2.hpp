#pragma once

#include <cstddef>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "allocator_concepts.hpp"
#include "preprocessor/noexcept_release_def.hpp"
#include "preprocessor/platform_def.hpp"
#include "span_iterator.hpp"

namespace ml {
// Vector which can extend an existing allocation instead of allocating a new one
template <typename T, typename Allocator>
    requires (extendable_allocator<Allocator> && can_allocate_bytes<Allocator>)
class vector2 {
  public:
    using value_type = T;
    using allocator_type = Allocator;
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

    inline static constexpr bool elems_must_be_destroyed =
        std::is_destructible_v<value_type> && !std::is_trivially_destructible_v<value_type>;

    // Constructors
    vector2() noexcept = default;
    vector2(ml::pmr* resource)
        : allocator_{resource} {}
    ~vector2() {
        destroy_all_elements();
        allocator_.deallocate(data_, capacity_);
    }

    // Element access
    template <typename Self>
    auto at(this Self&& self, size_type index) -> auto& {
        self.index_check(index);
        return std::forward<Self>(self).data_[index];
    }
    template <typename Self>
    auto back(this Self&& self) NOEXCEPT_RELEASE -> auto& {
        auto const i{self.size_ - 1};
        self.index_check_DEBUG(i);
        return std::forward<Self>(self).data_[i];
    }
    template <typename Self>
    auto data(this Self&& self) NOEXCEPT_RELEASE -> auto* {
        return std::forward<Self>(self).data_;
    }
    template <typename Self>
    auto front(this Self&& self) NOEXCEPT_RELEASE -> auto& {
        self.index_check_DEBUG(0);
        return std::forward<Self>(self).data_[0];
    }
    template <typename Self>
    auto operator[](this Self&& self, size_type index) NOEXCEPT_RELEASE->auto& {
        self.index_check_DEBUG(index);
        return std::forward<Self>(self).data_[index];
    }

    // Iterators
    auto begin() -> iterator { return iterator(data_); }
    auto begin() const -> const_iterator { return cbegin(); }
    auto cbegin() const -> const_iterator { return const_iterator(data_); }
    auto cend() const -> const_iterator { return const_iterator(data_ + size_); }
    auto crbegin() const -> const_reverse_iterator { return const_reverse_iterator(cend()); }
    auto crend() const -> const_reverse_iterator { return const_reverse_iterator(cbegin()); }
    auto end() -> iterator { return iterator(data_ + size_); }
    auto end() const -> const_iterator { return cend(); }
    auto rbegin() -> reverse_iterator { return reverse_iterator(end()); }
    auto rbegin() const -> const_reverse_iterator { return const_reverse_iterator(end()); }
    auto rend() -> reverse_iterator { return reverse_iterator(begin()); }
    auto rend() const -> const_reverse_iterator { return crend(); }

    // Capacity
    auto capacity() const noexcept -> size_type { return capacity_; }
    auto empty() const noexcept -> bool { return size_ == 0; }
    constexpr auto max_size() const noexcept -> size_type {
        return std::numeric_limits<difference_type>::max() / sizeof(int);
    }
    auto full() const noexcept -> bool { return size_ == capacity_; }
    auto size() const noexcept -> size_type { return size_; }

    // Modifiers
    void clear() {
        destroy_all_elements();
        size_ = 0;
    }
    template <typename... Args>
    void emplace_back(Args&&... args) {
        grow_if_full();
        new (data_ + size_) value_type{std::forward<Args>(args)...};
        ++size_;
    }
    void pop_back() {
        index_check_DEBUG(0);

        --size_;
        if constexpr (elems_must_be_destroyed) {
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
    // Element access
#ifdef DEBUG_ENABLED
    void index_check_DEBUG(size_type i) const { index_check(i); }
#else
    void index_check_DEBUG(size_type) const noexcept {}
#endif

    void index_check(size_type i) const {
        if (i >= size_) {
            throw std::out_of_range{"Index out of range"};
        }
    }
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
    // Modifiers
    void destroy_all_elements() {
        if constexpr (elems_must_be_destroyed) {
            for (size_type i{0}; i < size_; ++i) {
                data_[i].~value_type();
            }
        }
    }

    NO_UNIQUE_ADDRESS allocator_type allocator_;
    size_type size_{0};
    size_type capacity_{0};
    pointer data_{nullptr};
};
}

#include "preprocessor/platform_undef.hpp"
#include "preprocessor/noexcept_release_undef.hpp"
