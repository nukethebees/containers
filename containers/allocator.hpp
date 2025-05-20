#pragma once

#include <cstddef>
#include <memory>
#include <new>

#include "allocator_concepts.hpp"

namespace ml {
// Inherited std::allocator to add allocate_bytes and deallocate_bytes
template <typename T>
class allocator : public std::allocator<T> {
  public:
    using parent_allocator = std::allocator<T>;
    using size_type = typename parent_allocator::size_type;

    using std::allocator<T>::allocator;

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
