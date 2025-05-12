#pragma once

#include <cstddef>
#include <memory>
#include <new>

namespace ml {
template <typename T>
concept can_allocate_bytes = requires(T t, void* ptr, std::size_t size, std::size_t alignment) {
    { t.allocate_bytes(size, alignment) } -> std::same_as<void*>;
    { t.deallocate_bytes(ptr, size, alignment) } -> std::same_as<void>;
};

// Inherited std::allocator to add allocate_bytes and deallocate_bytes
template <typename T>
class allocator : public std::allocator<T> {
  public:
    using parent_allocator = std::allocator<T>;
    using size_type = typename parent_allocator::size_type;

    allocator() = default;

    auto allocate_bytes(size_type size, size_type alignment = alignof(std::max_align_t)) -> void*;
    void deallocate_bytes(void* ptr, size_type size, size_type alignment = alignof(std::max_align_t));
};

template <typename T>
inline auto allocator<T>::allocate_bytes(size_type size, size_type alignment) -> void* {
    return ::operator new(size, std::align_val_t{alignment});
}

template <typename T>
inline void allocator<T>::deallocate_bytes(void* ptr, size_type size, size_type alignment) {
    ::operator delete(ptr, size, std::align_val_t{alignment});
}

}
