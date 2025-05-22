#pragma once

#include <cstddef>
#include <memory_resource>
#include <utility>
#include <vector>

#include "allocator_concepts.hpp"

#include "preprocessor/platform_def.hpp"

namespace ml {
template <typename T, typename Comparator = std::less<>, typename Allocator = std::pmr::polymorphic_allocator<T>>
    requires ml::can_allocate_bytes<Allocator>
class binary_heap {
  public:
    using value_type = T;
    using allocator_type = Allocator;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    binary_heap() noexcept = default;
    binary_heap(Allocator const& alloc) noexcept
        : allocator_{alloc} {}

    // Access
    template <typename Self>
    auto parent(this Self&& self, size_type i) noexcept -> auto& {
        return std::forward<Self>(self).data_[parent_index(i)];
    }
    template <typename Self>
    auto left_child(this Self&& self, size_type i) noexcept -> auto& {
        return std::forward<Self>(self).data_[left_child_index(i)];
    }
    template <typename Self>
    auto right_child(this Self&& self, size_type i) noexcept -> auto& {
        return std::forward<Self>(self).data_[right_child_index(i)];
    }
    template <typename Self>
    auto operator[](this Self&& self, size_type i) noexcept -> auto& {
        return std::forward<Self>(self).data_[i];
    }

    // Capacity
    auto empty() const noexcept -> bool { return data_.empty(); }
    auto reserve(size_type n) -> void { data_.reserve(n); }
    auto size() const noexcept -> size_type { return data_.size(); }

    // Modifiers
    void clear() noexcept { data_.clear(); }
    template <typename U>
    void insert(U&& elem) {
        data_.push_back(std::forward<U>(elem));
        heapify(data_.size() - 1);
    }
    auto pop() {
        // Replace root with the last element
        std::swap(data_[0], data_.back());
        auto popped_elem{std::move(data_.back())};
        data_.pop_back();

        // Heapify the last node
        auto const n{data_.size()};
        size_type i{0};
        if (n) {
            auto update{[this, &i](auto& node, size_type idx) -> void {
                auto& child{this->data_[idx]};
                if (comparator_(child, node)) {
                    std::swap(child, node);
                    i = idx;
                }
            }};

            while (true) {
                auto original_idx{i};
                auto l_idx{left_child_index(i)};
                auto r_idx{right_child_index(i)};
                auto& node{data_[i]};

                auto r_valid{valid_index(r_idx)};
                auto l_valid{valid_index(l_idx)};

                // Check if parent is in the wrong position
                if (r_valid && l_valid) {
                    auto const smallest_idx{comparator_(data_[l_idx], data_[r_idx]) ? l_idx : r_idx};
                    update(node, smallest_idx);
                } else if (r_valid) {
                    update(node, r_idx);
                } else if (l_valid) {
                    update(node, l_idx);
                }

                if (i == original_idx) {
                    // No swap made
                    break;
                }
            }
        }

        return popped_elem;
    }
  private:
    // Access
    auto parent_index(size_type i) const noexcept -> size_type { return (i - 1) / 2; }
    auto left_child_index(size_type i) const noexcept -> size_type { return (2 * i) + 1; }
    auto right_child_index(size_type i) const noexcept -> size_type { return (2 * i) + 2; }
    auto valid_index(size_type i) const noexcept -> bool { return i < data_.size(); }
    // Modifiers
    void heapify(size_type i) noexcept {
        while (i > 0) {
            auto p_idx{parent_index(i)};
            // Parent and element
            auto& p{data_[p_idx]};
            auto& e{data_[i]};
            if (comparator_(e, p)) {
                std::swap(e, p);
                i = p_idx;
            } else {
                break;
            }
        }
    }

    NO_UNIQUE_ADDRESS Allocator allocator_;
    NO_UNIQUE_ADDRESS Comparator comparator_;
    std::vector<T, Allocator> data_;
};
}

#include "preprocessor/platform_undef.hpp"
