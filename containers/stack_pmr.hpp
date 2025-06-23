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

class stack_pmr : public std::pmr::memory_resource {
  public:
    using size_type = std::size_t;

    stack_pmr() = default;
    ~stack_pmr() override = default;

    // Capacity
    template <typename Self>
    auto buffer(this Self&& self) {
        return std::forward<Self>(self).buffer_;
    }
    auto empty() const -> bool { return size_ == 0; }
    auto size() const { return size_; }
    auto capacity() const { return capacity_; }
    void reserve(size_type n) {
        if (buffer_) {
            throw std::runtime_error("Buffer already reserved.");
        }
        buffer_ = std::make_unique<std::byte[]>(n);
        capacity_ = n;
    }

    // Allocation
    auto do_allocate(size_type n_bytes, size_type alignment) -> void* override final {
        return nullptr;
    }
    void do_deallocate(void* ptr, size_type n_bytes, size_type alignment) override final { return; }
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool override final {
        return this == &other;
    }
  private:
    std::unique_ptr<std::byte[]> buffer_{nullptr};
    size_type size_{0};
    size_type capacity_{0};
};

class stack_pmr_frame {
  public:
    using size_type = std::size_t;
  private:
    stack_frame_id id_;
    size_type size_{0};
};
}
