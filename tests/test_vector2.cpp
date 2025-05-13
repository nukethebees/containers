#include <gtest/gtest.h>

#include "containers/vector2.hpp"
#include "containers/arena_memory_resource.hpp"
#include "containers/arena_allocator.hpp"
#include "containers/memory_resource_allocator.hpp"

#include "configure_warning_pragmas.hpp"

template <typename T>
using arena_vec = ml::vector2<T, ml::MemoryResourceAllocator<T, ml::ArenaAllocator<T>>>;

using intvec = arena_vec<int>;

// Construction
TEST(vector2, init_empty) {
    intvec values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
