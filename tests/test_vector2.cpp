#include <gtest/gtest.h>

#include "containers/arena_allocator.hpp"
#include "containers/arena_memory_resource.hpp"
#include "containers/memory_resource.hpp"
#include "containers/memory_resource_allocator.hpp"
#include "containers/polymorphic_allocator.hpp"
#include "containers/stack_buffer_memory_resource_pmr.hpp"
#include "containers/vector2.hpp"

#include "configure_warning_pragmas.hpp"

template <typename T>
using arena_vec = ml::vector2<T, ml::polymorphic_allocator<T>>;

template <typename T, std::size_t CAPACITY>
using stack_pmr = ml::stack_buffer_pmr<T, CAPACITY, ml::memory_resource>;

using intvec = arena_vec<int>;

// smr = stack memory resource
TEST(vector2, smr_init_empty) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(vector2, smr_push_back_int) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};

    values.push_back(1);

    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(vector2, smr_push_back_int_multiple) {
    // Force multiple extensions
    stack_pmr<int, 100> resource;
    intvec values{&resource};

    static constexpr std::size_t n_elems{10};

    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }

    EXPECT_EQ(values.size(), n_elems);
    EXPECT_FALSE(values.empty());
}
TEST(vector2, smr_push_back_int_oob) {
    // Force multiple extensions
    stack_pmr<int, 1> resource;
    intvec values{&resource};

    values.push_back(1);
    EXPECT_THROW(values.push_back(1), std::bad_alloc);
}
