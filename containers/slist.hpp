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
    auto empty() const -> bool { return size_ == 0; }
    auto& front() { return node_->elem(); }
    auto& front() const { return node_->elem(); }
    auto size() const { return size_; }
};
}
