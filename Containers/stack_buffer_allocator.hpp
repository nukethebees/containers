#pragma once

#include <array>
#include <cstddef>

namespace ml {
template <typename T, std::size_t capacity>
class stack_buffer_allocator {
private:
    alignas(T) std::array<std::byte, capacity * sizeof(T)> storage{};
    std::size_t index{0};
public:
    constexpr stack_buffer_allocator() noexcept = default;
    template <class U>
    constexpr stack_buffer_allocator(const stack_buffer_allocator<U, capacity> &) noexcept {}

    constexpr T * allocate(std::size_t n) {
        auto const bytes_to_allocate{n * sizeof(T)};
        auto const space_remaining{storage.size() - index};

        if (bytes_to_allocate > space_remaining) {
            throw std::bad_alloc{};
        }

        auto * ptr{reinterpret_cast<T *>(&storage[index])};
        index += bytes_to_allocate;
        return ptr;
    }

    void deallocate(T *, std::size_t) {
        return;
    }

    template <class U>
    constexpr auto operator==(const stack_buffer_allocator<U, capacity> &) const noexcept -> bool {
        return true;
    }

    template <class U>
    constexpr auto operator!=(const stack_buffer_allocator<U, capacity> &) const noexcept -> bool {
        return false;
    }

    template<class U>
    struct rebind {
        using other = stack_buffer_allocator<U, capacity>;
    };
};
}