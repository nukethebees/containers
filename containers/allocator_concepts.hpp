#pragma once

#include <cstddef>

namespace ml {
template <typename T>
concept can_allocate_bytes = requires(T t, void* ptr, std::size_t size, std::size_t alignment) {
    { t.allocate_bytes(size, alignment) } -> std::same_as<void*>;
    { t.deallocate_bytes(ptr, size, alignment) } -> std::same_as<void>;
};

template <typename T>
concept extendable_allocator =
    requires(T t, void* ptr, std::size_t old_bytes, std::size_t new_bytes, std::size_t alignment) {
        { t.extend_bytes(ptr, old_bytes, new_bytes, alignment) } -> std::same_as<void*>;
    };

template <typename T>
concept extendable_memory_resource =
    requires(T t, void* ptr, std::size_t old_bytes, std::size_t new_bytes, std::size_t alignment) {
        { t.extend(ptr, old_bytes, new_bytes, alignment) } -> std::same_as<void*>;
    };
}
