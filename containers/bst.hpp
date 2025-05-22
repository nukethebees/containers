#pragma once

#include <functional>
#include <memory>
#include <type_traits>

#include "bst_iterator.hpp"
#include "bst_node.hpp"
#include "preprocessor/platform_def.hpp"

// Binary search tree
namespace ml {
template <typename T, typename Compare = std::less<>, template <typename> typename Allocator = std::allocator>
class bst {
  public:
    using value_type = T;
    using node_type = detail::bst_node<value_type>;
    using size_type = std::size_t;
    using allocator_type = Allocator<node_type>;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = bst_iterator<node_type, Compare>;
    using const_iterator = bst_iterator<node_type const, Compare>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    struct parent_child_pair {
        node_type* parent{nullptr};
        node_type** child{nullptr};

        parent_child_pair(node_type* parent_, node_type** child_)
            : parent{parent_}
            , child{child_} {}
    };

    // Access
    template <typename U>
        requires detail::bst_can_be_compared<T, U, Compare>
    auto contains(U&& value) const -> bool;
    template <typename Self>
    auto* max(this Self&& self);
    template <typename Self>
    auto* min(this Self&& self);
    auto root_value() const -> const_pointer;

    // Capacity
    auto empty() const -> bool;
    auto size() const -> size_type;

    // Iterators
    auto begin() -> iterator;
    auto begin() const -> const_iterator;
    auto cbegin() const -> const_iterator;
    auto cend() const -> const_iterator;
    auto crbegin() const -> const_reverse_iterator;
    auto crend() const -> const_reverse_iterator;
    auto end() -> iterator;
    auto end() const -> const_iterator;
    auto rbegin() -> reverse_iterator;
    auto rbegin() const -> const_reverse_iterator;
    auto rend() const -> const_reverse_iterator;
    auto rend() -> reverse_iterator;

    // Modifiers
    void clear();
    template <typename U>
    void insert(U&& value);
    // Caller needs to nullify the node afterwards
    void remove_from(node_type* node);
    void remove_from(const_reference value);
  private:
    // Access
    auto root() -> node_type*;
    auto root() const -> node_type const*;
    auto get_placement_address(const_reference value) -> parent_child_pair;
    template <typename Self>
    auto* max_node(this Self&& self);
    template <typename Self>
    auto* min_node(this Self&& self);

    // Modifiers
    void remove_from_inner(node_type* node);

    inline static Compare compare{};
    NO_UNIQUE_ADDRESS allocator_type alloc_;
    node_type* root_{nullptr};
    size_type size_{0};
};

#define METHOD_START(...)                                                           \
    template <typename T, typename Compare, template <typename> typename Allocator> \
    __VA_OPT__(__VA_ARGS__)                                                         \
    inline auto bst<T, Compare, Allocator>

// Access
template <typename T, typename Compare, template <typename> typename Allocator>
template <typename U>
    requires detail::bst_can_be_compared<T, U, Compare>
inline auto bst<T, Compare, Allocator>::contains(U&& value) const -> bool {
    auto* current{root_};
    while (current) {
        auto const& node_value{current->value()};
        if (compare(value, node_value)) {
            current = current->less();
        } else if (compare(node_value, std::forward<U>(value))) {
            current = current->greater();
        } else {
            return true;
        }
    }
    return false;
}
template <typename T, typename Compare, template <typename> typename Allocator>
template <typename Self>
inline auto* bst<T, Compare, Allocator>::max(this Self&& self) {
    auto* ptr{std::forward<Self>(self).max_node()};
    return ptr ? &ptr->value() : nullptr;
}
template <typename T, typename Compare, template <typename> typename Allocator>
template <typename Self>
inline auto* bst<T, Compare, Allocator>::min(this Self&& self) {
    auto* ptr{std::forward<Self>(self).min_node()};
    return ptr ? &ptr->value() : nullptr;
}
METHOD_START()::root_value() const->const_pointer {
    if (root_) {
        return &root_->value();
    }
    return nullptr;
}

// Capacity
METHOD_START()::empty() const->bool {
    return size_ == 0;
}
METHOD_START()::size() const->size_type {
    return size_;
}

// Iterators
METHOD_START()::begin()->iterator {
    auto* node{min_node()};
    if (!node) {
        return {};
    }
    return {node->parent(), node};
}
METHOD_START()::begin() const->const_iterator {
    auto* node{min_node()};
    if (!node) {
        return {};
    }
    return {node->parent(), node};
}
METHOD_START()::cbegin() const->const_iterator {
    auto* node{min_node()};
    if (!node) {
        return {};
    }
    return {node->parent(), node};
}
METHOD_START()::cend() const->const_iterator {
    return {max_node(), nullptr};
}
METHOD_START()::crbegin() const->const_reverse_iterator {
    return const_reverse_iterator{cend()};
}
METHOD_START()::crend() const->const_reverse_iterator {
    return const_reverse_iterator{cbegin()};
}
METHOD_START()::end()->iterator {
    return {max_node(), nullptr};
}
METHOD_START()::end() const->const_iterator {
    return {max_node(), nullptr};
}
METHOD_START()::rbegin()->reverse_iterator {
    return reverse_iterator{end()};
}
METHOD_START()::rbegin() const->const_reverse_iterator {
    return const_reverse_iterator{end()};
}
METHOD_START()::rend()->reverse_iterator {
    return reverse_iterator{begin()};
}
METHOD_START()::rend() const->const_reverse_iterator {
    return const_reverse_iterator{begin()};
}

// Modifiers
METHOD_START()::clear()->void {
    if (root_) {
        remove_from(root_);
        root_ = nullptr;
    }
}
METHOD_START(template <typename U>)::insert(U&& new_value)->void {
    auto address{get_placement_address(new_value)};

    auto& child{*address.child};
    if (!child) {
        child = alloc_.allocate(1);
        new (child) node_type(address.parent, std::forward<U>(new_value));
        size_++;
    }
}
METHOD_START()::remove_from(node_type* node)->void {
    if (!node) {
        return;
    }

    auto* parent{node->parent()};
    auto& less{node->less()};
    auto& greater{node->greater()};

    if (less) {
        remove_from_inner(less);
    }
    if (greater) {
        remove_from_inner(greater);
    }

    alloc_.deallocate(node, 1);
    size_--;

    if (parent) {
        parent->disconnect(node);
    } else if (node == root_) {
        root_ = nullptr;
    }
}
METHOD_START()::remove_from(const_reference value)->void {
    auto address{get_placement_address(value)};
    auto& child{*address.child};
    if (child) {
        remove_from(child);
    }
}

// Private methods
// Access
METHOD_START()::root()->node_type* {
    return root_;
}
METHOD_START()::root() const->node_type const* {
    return root_;
}
METHOD_START()::get_placement_address(const_reference value)->parent_child_pair {
    node_type* parent{nullptr};
    node_type* current{root_};
    node_type** address{&root_};

    while (current) {
        auto const& node_value{current->value()};
        if (compare(value, node_value)) {
            auto& ptr{current->less()};

            parent = current;
            address = &ptr;
            current = ptr;
        } else if (compare(node_value, value)) {
            auto& ptr{current->greater()};

            parent = current;
            address = &ptr;
            current = ptr;
        } else {
            break;
        }
    }

    return parent_child_pair{parent, address};
}
template <typename T, typename Compare, template <typename> typename Allocator>
template <typename Self>
inline auto* bst<T, Compare, Allocator>::max_node(this Self&& self) {
    auto* current{std::forward<Self>(self).root_};

    while (current) {
        auto* next{current->greater()};
        if (next) {
            current = next;
        } else {
            break;
        }
    }

    return current;
}
template <typename T, typename Compare, template <typename> typename Allocator>
template <typename Self>
inline auto* bst<T, Compare, Allocator>::min_node(this Self&& self) {
    auto* current{std::forward<Self>(self).root_};

    while (current) {
        auto* next{current->less()};
        if (next) {
            current = next;
        } else {
            break;
        }
    }

    return current;
}

// Modifiers
METHOD_START()::remove_from_inner(node_type* node)->void {
    auto& less{node->less()};
    auto& greater{node->greater()};

    if (less) {
        remove_from(less);
        less = nullptr;
    }

    if (greater) {
        remove_from(greater);
        greater = nullptr;
    }

    alloc_.deallocate(node, 1);
    size_--;
}

#undef METHOD_START
}

#include "preprocessor/platform_undef.hpp"
