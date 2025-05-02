#pragma once

#include <cstddef>
#include <functional>
#include <memory>

#include "platform_def.hpp"

// Binary search tree
namespace ml {
namespace detail {
template <typename T>
class bst_node {
  public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    bst_node() = delete;
    bst_node(bst_node* parent, T&& value) noexcept;

    ~bst_node() = default;

    auto greater() -> bst_node*&;
    auto greater() const -> bst_node const*&;
    auto less() -> bst_node*&;
    auto less() const -> bst_node const*&;
    auto parent() -> bst_node*&;
    auto parent() const -> bst_node const*&;
    auto value() -> reference;
    auto value() const -> const_reference;
    auto operator*() -> reference;
    auto operator*() const -> const_reference;

    void disconnect(bst_node* node);
  private:
    T value_;
    bst_node* less_{nullptr};
    bst_node* greater_{nullptr};
    bst_node* parent_{nullptr};
};

template <typename T>
bst_node<T>::bst_node(bst_node* parent, T&& value) noexcept
    : value_{std::move(value)}
    , parent_{parent} {}

#define METHOD_START      \
    template <typename T> \
    inline auto bst_node<T>

METHOD_START::greater()->bst_node*& {
    return greater_;
}
METHOD_START::greater() const->bst_node const*& {
    return greater_;
}
METHOD_START::less()->bst_node*& {
    return less_;
}
METHOD_START::less() const->bst_node const*& {
    return less_;
}
METHOD_START::parent()->bst_node*& {
    return parent_;
}
METHOD_START::parent() const->bst_node const*& {
    return parent_;
}
METHOD_START::value()->reference {
    return value_;
}
METHOD_START::value() const->const_reference {
    return value_;
}
METHOD_START::operator*()->reference {
    return value_;
}
METHOD_START::operator*() const->const_reference {
    return value_;
}

METHOD_START::disconnect(bst_node* node)->void {
    if (less_ == node) {
        less_ = nullptr;
    } else if (greater_ == node) {
        greater_ = nullptr;
    }
}

#undef METHOD_START

template <typename T, typename U, typename Compare>
concept bst_can_be_compared = requires(T t, U u) {
    { Compare{}(u, t) } -> std::convertible_to<bool>;
    { Compare{}(t, u) } -> std::convertible_to<bool>;
};
}

template <typename T>
class bst_iterator {
  public:
    using value_type = T;
    using node_type = detail::bst_node<value_type>;

    bst_iterator() noexcept = default;
    bst_iterator(node_type* ptr) noexcept
        : ptr_{ptr} {}
  private:
    node_type* ptr_{nullptr};
};

template <typename T, typename Compare = std::less<T>, template <typename> typename Allocator = std::allocator>
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
    using iterator = bst_iterator<value_type>;
    using const_iterator = bst_iterator<value_type const>;

    struct parent_child_pair {
        node_type* parent{nullptr};
        node_type** child{nullptr};

        parent_child_pair(node_type* parent, node_type** child)
            : parent{parent}
            , child{child} {}
    };

    // Access
    template <typename U>
        requires detail::bst_can_be_compared<T, U, Compare>
    auto contains(U&& value) const -> bool;
    template <typename Self>
    auto* max(this Self&& self);
    template <typename Self>
    auto* min(this Self&& self);
    auto root() -> pointer;
    auto root() const -> const_pointer;

    // Capacity
    auto empty() const -> bool;
    auto size() const -> size_type;

    // Modifiers
    void clear();
    template <typename U>
    void insert(U&& value);
    // Caller needs to nullify the node afterwards
    void remove_from(node_type* node);
    void remove_from(const_reference value);
  private:
    auto get_placement_address(const_reference value) -> parent_child_pair;
    template <typename Self>
    auto* max_node(this Self&& self);
    template <typename Self>
    auto* min_node(this Self&& self);

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
METHOD_START()::root()->pointer {
    if (!root_) {
        return nullptr;
    }
    return &root_->value();
}
METHOD_START()::root() const->const_pointer {
    if (!root_) {
        return nullptr;
    }
    return &root_->value();
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

METHOD_START()::empty() const->bool {
    return size_ == 0;
}
METHOD_START()::size() const->size_type {
    return size_;
}

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
        new (child) node_type{address.parent, std::forward<U>(new_value)};
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

#include "platform_undef.hpp"
