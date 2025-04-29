#pragma once

#include <cstddef>
#include <memory>
#include <utility>

namespace ml {
// Singly-linked list
template <typename T>
class slist {
    struct node {
        T elem_;
        std::unique_ptr<node> next_{nullptr};

        node() = default;
        template <typename... Args>
        node(Args&&... args)
            : elem_{std::forward<Args>(args)...} {}
        auto& elem() { return elem_; }
        auto& elem() const { return elem_; }
    };

    std::unique_ptr<node> node_{nullptr};
    std::size_t size_{0};
    node* tail_{nullptr};
  public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const*;

    slist() = default;

    auto back() -> reference { return tail_->elem(); }
    auto back() const -> const_reference { return tail_->elem(); }
    void clear() {
        node_.reset();
        tail_ = nullptr;
        size_ = 0;
    }
    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (!tail_) {
            node_ = std::make_unique<node>(std::forward<Args>(args)...);
            tail_ = node_.get();
        } else {
            tail_->next_ = std::make_unique<node>(std::forward<Args>(args)...);
            tail_ = tail_->next_.get();
        }
        size_++;
        return;
    }
    template <typename... Args>
    void emplace_front(Args&&... args) {
        auto new_node{std::make_unique<node>(std::forward<Args>(args)...)};
        new_node->next_ = std::move(node_);
        node_ = std::move(new_node);
        if (!tail_) {
            tail_ = node_.get();
        }
        size_++;
    }
    auto empty() const -> bool { return size_ == 0; }
    auto front() -> reference { return node_->elem(); }
    auto front() const -> const_reference { return node_->elem(); }
    // Inserts an element at the given position
    // If the position is greater than the size of the list, it will be inserted at the end
    template <typename U>
    void insert(U&& new_elem, std::size_t i) {
        auto const size{size_};
        if (i > size) {
            i = size;
        }

        if (i == 0) {
            push_front(std::forward<U>(new_elem));
            return;
        }

        if (i == size) {
            push_back(std::forward<U>(new_elem));
            return;
        }

        // Insert the new element between i-1 and i
        // (i-1) -> (i)
        // (i-1) -> (new_node) -> (i)
        auto new_node{std::make_unique<node>(std::forward<U>(new_elem))};
        auto* prev{node_at(i - 1)};

        new_node->next_ = std::move(prev->next_);
        prev->next_ = std::move(new_node);
        size_++;

        return;
    }
    void pop_back() {
        if (empty()) {
            return;
        }

        if (size_ == 1) {
            node_.reset();
            tail_ = nullptr;
        } else {
            auto* prev{node_.get()};
            while (prev->next_->next_) {
                prev = prev->next_.get();
            }
            prev->next_.reset();
            tail_ = prev;
        }

        size_--;
    }
    void pop_front() {
        if (empty()) {
            return;
        }
        if (size_ == 1) {
            node_.reset();
            tail_ = nullptr;
        } else {
            node_ = std::move(node_->next_);
        }
        size_--;
    }
    template <typename U>
    void push_back(U&& new_elem) {
        if (!tail_) {
            node_ = std::make_unique<node>(std::forward<U>(new_elem));
            tail_ = node_.get();
        } else {
            tail_->next_ = std::make_unique<node>(std::forward<U>(new_elem));
            tail_ = tail_->next_.get();
        }
        size_++;
    }
    template <typename U>
    void push_front(U&& new_elem) {
        auto new_node{std::make_unique<node>(std::forward<U>(new_elem))};
        new_node->next_ = std::move(node_);
        node_ = std::move(new_node);
        if (!tail_) {
            tail_ = node_.get();
        }
        size_++;
    }
    auto size() const -> size_type { return size_; }
  private:
    auto* node_at(std::size_t i) {
        auto* current{node_.get()};
        for (std::size_t j{0}; j < i; ++j) {
            current = current->next_.get();
        }
        return current;
    }
};
}
