#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "bst_node.hpp"

// Binary search tree
namespace ml {
template <typename NodeT, typename Compare>
class bst_iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using node_type = NodeT;
    using value_type = typename node_type::value_type;
    using pointer = decltype(&*std::declval<node_type>());
    using reference = decltype(*std::declval<node_type>());
    using iterator_category = std::bidirectional_iterator_tag;

    bst_iterator() noexcept = default;
    bst_iterator(node_type* parent, node_type* node) noexcept;

    auto operator*() const -> reference;
    auto operator*() -> reference;

    auto operator++() -> bst_iterator&;
    auto operator++(int) -> bst_iterator;
    auto operator--() -> bst_iterator&;
    auto operator--(int) -> bst_iterator;
    auto operator<=>(bst_iterator const& other) const = default;
  private:
    inline static Compare compare{};
    node_type* node_{nullptr};
    node_type* parent_{nullptr};
};

#define METHOD_START                        \
    template <typename T, typename Compare> \
    inline auto bst_iterator<T, Compare>

template <typename T, typename Compare>
inline bst_iterator<T, Compare>::bst_iterator(node_type* parent, node_type* node) noexcept
    : node_{node}
    , parent_{parent} {}

METHOD_START::operator*() const->reference {
    return node_->value();
}
METHOD_START::operator*()->reference {
    return node_->value();
}
METHOD_START::operator++()->bst_iterator& {
    // Follow the greater path until it's null
    // If there's no greater value then ascend to the next larger value

    // Local copy to keep it in registers
    auto* current_node{node_};

    if (current_node) {
        if (auto* new_node{current_node->greater()}) {
            auto* new_least_node{new_node->less()};

            if (new_least_node) {
                // Go to the smallest value in the greater path
                do {
                    new_node = new_least_node;
                    new_least_node = new_least_node->less();
                } while (new_least_node);
            }

            node_ = new_node;
            parent_ = new_node->parent();
        } else {
            // Ascend to the next larger value
            auto const& current_value{node_->value()};

            auto* new_parent{current_node->parent()};
            while (new_parent) {
                // Ascend to the parent with a value greater than the current value
                auto const& parent_value{new_parent->value()};

                // Check parent is greater than current value
                if (compare(current_value, parent_value)) {
                    current_node = new_parent;
                    break;
                }

                new_parent = new_parent->parent();
            }

            if (current_node->value() <= current_value) {
                // If the current value is the same as the parent value then we've reached the end
                // Assign the max value as the parent and nullify the node
                parent_ = node_;
                node_ = nullptr;
            } else {
                node_ = current_node;
            }
        }
    }

    return *this;
}
METHOD_START::operator++(int)->bst_iterator {
    auto temp{*this};
    ++(*this);
    return temp;
}
METHOD_START::operator--()->bst_iterator& {
    // Reverse the process of ++

    if (node_) {
        auto* next{node_->less()};

        if (next) {
            while (next->greater()) {
                next = next->greater();
            }

            node_ = next;
        } else {
            auto* current{node_};
            auto const& current_value{node_->value()};

            while (current->parent()) {
                auto const& parent_value{current->parent()->value()};

                if (compare(parent_value, current_value)) {
                    current = current->parent();
                    break;
                }

                current = current->parent();
            }

            if (current->value() >= current_value) {
                // Must be at the beginning
            } else {
                node_ = current;
                parent_ = node_->parent();
            }
        }
    } else {
        if (parent_) {
            // If the parent is not null, assign the parent to the node
            // This should only happen when we're at the max value
            node_ = parent_;
            if (node_->parent()) {
                parent_ = node_->parent();
            }
        }
    }

    return *this;
}
METHOD_START::operator--(int)->bst_iterator {
    auto temp{*this};
    --(*this);
    return temp;
}

namespace detail {
using example_bst_node = bst_node<int>;
using example_bst_iterator = bst_iterator<example_bst_node, std::less<int>>;

static_assert(std::input_or_output_iterator<example_bst_iterator>);
static_assert(std::input_iterator<example_bst_iterator>);
static_assert(std::forward_iterator<example_bst_iterator>);
static_assert(std::bidirectional_iterator<example_bst_iterator>);
}

#undef METHOD_START
}
