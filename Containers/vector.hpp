#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

namespace ml {
template <typename T>
class vector {
private:
    T * data_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};
    static inline std::allocator<T> alloc{};

    void grow(this vector & self) {
        self.reserve(self.capacity_ ? self.capacity_ * 2uz : 1uz);
    }
public:
    vector() noexcept = default;

    template <typename U>
        requires std::is_same_v<std::remove_cvref_t<U>, T>
    void emplace_back(this vector & self, U && element) {
        auto const new_size{self.size_ + 1};

        if (new_size >= self.capacity_) {
            self.grow();
        }

        new (self.data_ + self.size_) T{std::forward<U>(element)};
        self.size_++;
    }

    void resize(this vector & self, std::size_t new_size) {
        //auto * new_data = alloc.allocate(new_size);
        //
        //alloc.deallocate(data_, self.capacity);
        //self.data_ = new_data;
        //self.capacity_ = new_size;
    }
    void reserve(this vector & self, std::size_t new_capacity) {
        if (new_capacity <= self.capacity_) {
            return;
        }

        auto * new_data = alloc.allocate(new_capacity);

        for (auto i{0uz}; i < self.size_; i++) {
            new (new_data + i) T{std::move(self.data_[i])};
        }

        alloc.deallocate(self.data_, self.capacity_);
        self.data_ = new_data;
        self.capacity_ = new_capacity;
    }

    auto size(this vector const & self) -> std::size_t {
        return self.size_;
    }
    auto capacity(this vector const & self) -> std::size_t {
        return self.capacity_;
    }

    template <typename Self>
    auto operator[](this Self && self, std::size_t idx) {
        return std::forward<Self>(self).data_[idx];
    }
};
}