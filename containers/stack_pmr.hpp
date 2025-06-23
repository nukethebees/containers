#pragma once

#include <cstddef>
#include <cstdint>
#include <memory_resource>
#include <optional>
#include <stdexcept>
#include <utility>
#include <memory>

namespace ml {
using stack_frame_id = uint16_t;
class stack_pmr_frame;

using unique_frame_ptr = std::unique_ptr<stack_pmr_frame, void (*)(stack_pmr_frame*)>;

class stack_pmr {
  public:
    using size_type = std::size_t;

    stack_pmr() = default;
    ~stack_pmr() = default;

    // Capacity
    template <typename Self>
    auto buffer(this Self&& self) {
        return std::forward<Self>(self).buffer_;
    }
    auto empty() const -> bool { return size_ == 0; }
    auto size() const { return size_; }
    auto capacity() const { return capacity_; }
    auto remaining_capacity() const { return capacity() - size(); }
    void reserve(size_type n) {
        if (buffer_) {
            throw std::runtime_error("Buffer already reserved.");
        }
        buffer_ = std::make_unique<std::byte[]>(n);
        capacity_ = n;
    }

    // Frames
    auto create_frame() -> unique_frame_ptr;
    void pop_frame(stack_pmr_frame* frame);

    // Allocation
    [[nodiscard]] auto allocate(stack_pmr_frame* frame, size_type n_bytes, size_type alignment)
        -> void* {
        if (frame != current_) {
            throw std::runtime_error("Non-current frame tried to allocate.");
        }
        return allocate(n_bytes, alignment);
    }
  private:
    [[nodiscard]] auto allocate(size_type n_bytes, size_type alignment) -> void* {
        auto const cur_size{size_};
        auto remaining{remaining_capacity()};

        auto* new_start{static_cast<void*>(buffer_.get() + cur_size)};
        if (!std::align(alignment, n_bytes, new_start, remaining)) {
            throw std::bad_alloc{};
        }

        size_ = (capacity_ - remaining) + n_bytes;
        return new_start;
    }

    std::unique_ptr<std::byte[]> buffer_{nullptr};
    size_type size_{0};
    size_type capacity_{0};
    stack_pmr_frame* current_{nullptr};
};

class stack_pmr_frame : public std::pmr::memory_resource {
  public:
    using size_type = std::size_t;
    friend class stack_pmr;

    stack_pmr_frame() = delete;
    stack_pmr_frame(stack_pmr* stack, stack_pmr_frame* previous, size_type previous_size)
        : stack_{stack}
        , previous_{previous}
        , previous_size_{previous_size} {}

    ~stack_pmr_frame() override {
        if (stack_) {
            stack_->pop_frame(this);
        }
    }

    stack_pmr_frame(stack_pmr_frame const&) = delete;
    stack_pmr_frame(stack_pmr_frame&&) = delete;

    auto& operator=(stack_pmr_frame const&) = delete;
    auto& operator=(stack_pmr_frame&&) = delete;

    auto do_allocate(size_type n_bytes, size_type alignment) -> void* override final {
        return stack_->allocate(this, n_bytes, alignment);
    }
    void do_deallocate(void* /*ptr*/,
                       size_type /*n_bytes*/,
                       size_type /*alignment*/) override final {
        return;
    }
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool override final {
        return this == &other;
    }
  private:
    stack_pmr* stack_{nullptr};
    stack_pmr_frame* previous_{nullptr};
    size_type previous_size_{0};
};
inline auto stack_pmr::create_frame() -> unique_frame_ptr {
    auto old_size{size()};
    void* frame_address{allocate(sizeof(stack_pmr_frame), alignof(stack_pmr_frame))};
    auto* frame{new (frame_address) stack_pmr_frame{this, current_, old_size}};
    current_ = frame;
    return unique_frame_ptr{frame, [](stack_pmr_frame* frame) { frame->~stack_pmr_frame(); }};
}
inline void stack_pmr::pop_frame(stack_pmr_frame* frame) {
    if (frame != current_) {
        throw std::runtime_error("Cannot pop non-current frame.");
    }
    if (!frame) {
        return;
    }

    current_ = frame->previous_;
    size_ = frame->previous_size_;
    return;
}
}
