#pragma once

#include "memory_resource_allocator.hpp"
#include "arena_mmr.hpp"

namespace ml {
template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, arena_mmr>;
}
