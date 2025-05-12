#pragma once

#include <cstddef>

namespace ml {
template <typename T>
concept can_allocate_bytes = requires(T t, void* ptr, std::size_t size, std::size_t alignment) {
    { t.allocate_bytes(size, alignment) } -> std::same_as<void*>;
    { t.deallocate_bytes(ptr, size, alignment) } -> std::same_as<void>;
};
}
