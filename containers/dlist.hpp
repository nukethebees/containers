#pragma once

#include "platform.hpp"

namespace ml {
// Doubly-linked list
template <typename T, typename Allocator = std::allocator<T>>
class dlist {
    struct Node {
        Node* prev_{nullptr};
        Node* next_{nullptr};
        T elem_{};

        auto operator*() -> T& { return elem_; }
        auto operator*() const -> T const& { return elem_; }
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

    auto back() noexcept -> reference { return *tail_; }
    auto back() const noexcept -> const_reference { return *tail_; }
    auto empty() const noexcept -> bool { return size_ == 0; }
    auto front() noexcept -> reference { return *head_; }
    auto front() const noexcept -> const_reference { return *head_; }
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
    }
    auto size() const noexcept -> size_type { return size_; }
  private:
    Node* head_{nullptr};
    Node* tail_{nullptr};
    size_type size_{0};
    NO_UNIQUE_ADDRESS Allocator alloc_{};
};
}
