#pragma once

#include <compare>
#include <cstddef>
#include <utility>

#include "linked_vector_segment.hpp"

namespace ml {
template <typename NodeT>
class linked_vector_iterator {
  public:
    using node_type = NodeT;
    using size_type = typename node_type::size_type;
    using difference_type = std::ptrdiff_t;
    using value_type = typename node_type::value_type;
    using pointer = decltype(&std::declval<node_type>().data[0]);
    using reference = decltype(std::declval<node_type>().data[0]);
    using iterator_category = std::bidirectional_iterator_tag;

    linked_vector_iterator() = default;
    linked_vector_iterator(node_type* node)
        : node_(node) {}
    linked_vector_iterator(node_type* prev, node_type* node, size_type index)
        : prev_(prev)
        , node_(node)
        , index_(index) {}
    ~linked_vector_iterator() = default;

    auto operator*() const -> reference { return node_->data[index_]; }
    auto operator*() -> reference { return node_->data[index_]; }
    auto operator->() const -> pointer { return &node_->data[index_]; }
    auto operator->() -> pointer { return &node_->data[index_]; }

    auto operator++() -> linked_vector_iterator& {
        ++index_;
        if (index_ >= node_->size) {
            prev_ = node_;
            node_ = node_->next;
            index_ = 0;
        }
        return *this;
    }
    auto operator++(int) -> linked_vector_iterator {
        auto temp{*this};
        ++(*this);
        return temp;
    }
    auto operator--() -> linked_vector_iterator& {
        if (!node_) {
            node_ = prev_;
            prev_ = node_->prev;
            index_ = node_->size - 1;
        } else if (index_ == 0) {
            node_ = node_->prev;
            index_ = node_->size - 1;
        } else {
            --index_;
        }
        return *this;
    }
    auto operator--(int) -> linked_vector_iterator {
        auto temp{*this};
        --(*this);
        return temp;
    }

    auto operator<=>(linked_vector_iterator const& other) const = default;
  private:
    node_type* prev_{nullptr};
    node_type* node_{nullptr};
    size_type index_{0};
};

namespace detail {
using example_linked_segment_node = linked_vector_segment<int>;
using example_linked_vector_iterator = linked_vector_iterator<example_linked_segment_node>;

static_assert(std::input_or_output_iterator<example_linked_vector_iterator>);
static_assert(std::input_iterator<example_linked_vector_iterator>);
static_assert(std::forward_iterator<example_linked_vector_iterator>);
static_assert(std::bidirectional_iterator<example_linked_vector_iterator>);
}
}
