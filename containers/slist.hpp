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
    slist() = default;

    auto& back() { return tail_->elem(); }
    auto& back() const { return tail_->elem(); }
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
    auto& front() { return node_->elem(); }
    auto& front() const { return node_->elem(); }
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
    auto size() const { return size_; }
};
}
