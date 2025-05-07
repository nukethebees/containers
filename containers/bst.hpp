#pragma once

#include <cstddef>
#include <compare>
#include <iterator>
#include <functional>
#include <memory>
#include <type_traits>

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

template <typename T, bool is_const, typename Compare>
class bst_iterator {
  public:
    using difference_type = std::ptrdiff_t;
    using element_type = std::remove_cvref_t<T>;
    using node_type =
        std::conditional_t<is_const, detail::bst_node<element_type> const, detail::bst_node<element_type>>;
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
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

#define METHOD_START                                       \
    template <typename T, bool is_const, typename Compare> \
    inline auto bst_iterator<T, is_const, Compare>

template <typename T, bool is_const, typename Compare>
inline bst_iterator<T, is_const, Compare>::bst_iterator(node_type* parent, node_type* node) noexcept
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

    if (node_) {
        auto* next{node_->greater()};

        if (next) {
            // Go to the smallest value in the greater path
            while (next->less()) {
                next = next->less();
            }

            node_ = next;
        } else {
            // Ascend to the next larger value
            auto* current{node_};
            auto const& current_value{node_->value()};

            while (current->parent()) {
                // Ascend to the parent with a value greater than the current value
                auto const& parent_value{current->parent()->value()};

                if (compare(current_value, parent_value)) {
                    current = current->parent();
                    break;
                }

                current = current->parent();
            }

            if (current->value() <= current_value) {
                // If the current value is the same as the parent value
                // We've reached the end
                // Assign the max value as the parent and nullify the node
                this->parent_ = node_;
                this->node_ = nullptr;
            } else {
                node_ = current;
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

    return *this;
}
METHOD_START::operator--(int)->bst_iterator {
    auto temp{*this};
    --(*this);
    return temp;
}

static_assert(std::input_or_output_iterator<bst_iterator<int, false, std::less<int>>>);
static_assert(std::input_iterator<bst_iterator<int, false, std::less<int>>>);
static_assert(std::forward_iterator<bst_iterator<int, false, std::less<int>>>);
static_assert(std::bidirectional_iterator<bst_iterator<int, false, std::less<int>>>);

#undef METHOD_START

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
    using iterator = bst_iterator<value_type, false, Compare>;
    using const_iterator = bst_iterator<value_type const, true, Compare>;
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

#include "platform_undef.hpp"
