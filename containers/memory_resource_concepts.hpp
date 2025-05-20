#pragma once

#include <cstddef>

namespace ml {
template <typename T>
concept extendable_memory_resource =
    requires(T t, void* ptr, std::size_t old_bytes, std::size_t new_bytes, std::size_t alignment) {
        { t.extend(ptr, old_bytes, new_bytes, alignment) } -> std::same_as<void*>;
    };
}
