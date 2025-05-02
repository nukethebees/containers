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
    bst_node(T&& value) noexcept;

    ~bst_node() = default;

    auto value() -> reference;
    auto value() const -> const_reference;
    auto less() -> bst_node*;
    auto less() const -> bst_node const*;
    auto greater() -> bst_node*;
    auto greater() const -> bst_node const*;

    auto operator*() -> reference;
    auto operator*() const -> const_reference;
  private:
    T value_;
    bst_node* less_{nullptr};
    bst_node* greater_{nullptr};
};

template <typename T>
bst_node<T>::bst_node(T&& value) noexcept
    : value_{std::move(value)} {}

#define METHOD_START      \
    template <typename T> \
    inline auto bst_node<T>

METHOD_START::value()->reference {
    return value_;
}
METHOD_START::value() const->const_reference {
    return value_;
}
METHOD_START::less()->bst_node* {
    return less_;
}
METHOD_START::less() const->bst_node const* {
    return less_;
}
METHOD_START::greater()->bst_node* {
    return greater_;
}
METHOD_START::greater() const->bst_node const* {
    return greater_;
}

METHOD_START::operator*()->reference {
    return value_;
}
METHOD_START::operator*() const->const_reference {
    return value_;
}

#undef METHOD_START

template <typename T, typename U, typename Compare>
concept bst_can_be_compared = requires(T t, U u) {
    { Compare{}(u, t) } -> std::convertible_to<bool>;
    { Compare{}(t, u) } -> std::convertible_to<bool>;
};
}

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

    // Access
    template <typename U>
        requires detail::bst_can_be_compared<T, U, Compare>
    auto contains(U&& value) const -> bool;
    auto root() -> pointer;
    auto root() const -> const_pointer;

    // Capacity
    auto empty() const -> bool;
    auto size() const -> bool;

    // Modifiers
    void clear();
    template <typename U>
    void insert(U&& value);
    // Caller needs to nullify the node afterwards
    void remove_from(node_type* node);
  private:
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

METHOD_START()::empty() const->bool {
    return size_ == 0;
}
METHOD_START()::size() const->bool {
    return size_;
}

METHOD_START()::clear()->void {
    if (root_) {
        remove_from(root_);
        root_ = nullptr;
    }
}
METHOD_START(template <typename U>)::insert(U&& new_value)->void {
    auto* current{root_};

    if (!current) {
        root_ = alloc_.allocate(1);
        new (root_) node_type{std::forward<U>(new_value)};
        size_++;
        return;
    }

    while (current) {
        auto const& node_value{current->value()};
        if (compare(new_value, node_value)) {
            current = current->less();
        } else if (compare(node_value, new_value)) {
            current = current->greater();
        } else {
            return;
        }
    }
}
METHOD_START()::remove_from(node_type* node)->void {
    if (!node) {
        return;
    }
    if (node->less()) {
        remove_from(node->less());
    }
    if (node->greater()) {
        remove_from(node->greater());
    }
    alloc_.deallocate(node, 1);
    size_--;
}

#undef METHOD_START
}

#include "platform_undef.hpp"
