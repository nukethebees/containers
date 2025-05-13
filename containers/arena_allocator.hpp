#pragma once

#include "memory_resource_allocator.hpp"
#include "arena_memory_resource.hpp"

namespace ml {
template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;
}
