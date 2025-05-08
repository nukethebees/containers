#pragma once

#include <utility>
#include <type_traits>

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
    template <typename U>
    bst_node(bst_node* parent, U&& value) noexcept;

    ~bst_node() = default;

    auto greater() -> bst_node*&;
    auto greater() const -> bst_node const*;
    auto less() -> bst_node*&;
    auto less() const -> bst_node const*;
    auto parent() -> bst_node*&;
    auto parent() const -> bst_node const*;
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
template <typename U>
bst_node<T>::bst_node(bst_node* parent, U&& value) noexcept
    : value_{std::forward<U>(value)}
    , parent_{parent} {}

#define METHOD_START      \
    template <typename T> \
    inline auto bst_node<T>

METHOD_START::greater()->bst_node*& {
    return greater_;
}
METHOD_START::greater() const->bst_node const* {
    return greater_;
}
METHOD_START::less()->bst_node*& {
    return less_;
}
METHOD_START::less() const->bst_node const* {
    return less_;
}
METHOD_START::parent()->bst_node*& {
    return parent_;
}
METHOD_START::parent() const->bst_node const* {
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
}
