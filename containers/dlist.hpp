#pragma once

#include <memory>
#include <utility>

#include "platform_def.hpp"

namespace ml {
// Doubly-linked list
template <typename T, template <typename> typename Allocator = std::allocator>
class dlist {
    struct Node {
        Node* prev_{nullptr};
        Node* next_{nullptr};
        T elem_{};

        auto operator*() -> T& { return elem_; }
        auto operator*() const -> T const& { return elem_; }

        Node() = default;
        template <typename... Args>
        Node(Args&&... args)
            : elem_{std::forward<Args>(args)...} {}
    };

    class Iterator {};
  public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;
    using iterator = Iterator;

    dlist() noexcept = default;

    auto back() noexcept -> reference { return **tail_; }
    auto back() const noexcept -> const_reference { return **tail_; }
    auto empty() const noexcept -> bool { return size_ == 0; }
    auto front() noexcept -> reference { return **head_; }
    auto front() const noexcept -> const_reference { return **head_; }
    void pop_back() {
        if (empty()) {
            return;
        }

        if (size_ == 1) {
            tail_->~Node();
            alloc_.deallocate(tail_, 1);
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            auto* prev{tail_->prev_};
            tail_->~Node();
            alloc_.deallocate(tail_, 1);
            tail_ = prev;
            tail_->next_ = nullptr;
        }

        size_--;
    }
    void pop_front() {
        if (empty()) {
            return;
        }

        if (size_ == 1) {
            head_->~Node();
            alloc_.deallocate(head_, 1);
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            auto* next{head_->next_};
            head_->~Node();
            alloc_.deallocate(head_, 1);
            head_ = next;
            head_->prev_ = nullptr;
        }

        size_--;
    }
    template <typename U>
    void push_back(U&& new_elem) {
        auto* new_node{alloc_.allocate(1)};
        auto* new_node_ptr{new (new_node) Node{std::forward<U>(new_elem)}};

        if (empty()) {
            head_ = new_node_ptr;
            tail_ = new_node_ptr;
        } else {
            tail_->next_ = new_node_ptr;
            new_node_ptr->prev_ = tail_;
            tail_ = new_node_ptr;
        }

        size_++;
    }
    template <typename U>
    void push_front(U&& new_elem) {
        auto* new_node{alloc_.allocate(1)};
        auto* new_node_ptr{new (new_node) Node{std::forward<U>(new_elem)}};

        if (empty()) {
            head_ = new_node_ptr;
            tail_ = new_node_ptr;
        } else {
            head_->prev_ = new_node_ptr;
            new_node_ptr->next_ = head_;
            head_ = new_node_ptr;
        }

        size_++;
    }
    auto size() const noexcept -> size_type { return size_; }
  private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    size_type size_{0};
    NO_UNIQUE_ADDRESS Allocator<Node> alloc_{};
};
}

#include "platform_undef.hpp"
