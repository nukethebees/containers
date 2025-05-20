#pragma once

#include "mmr_allocator.hpp"
#include "arena_mmr.hpp"

namespace ml {
template <typename T>
using arena_mmr_allocator = mmr_allocator<T, arena_mmr>;
}
