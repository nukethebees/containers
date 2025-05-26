#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/arena_mmr_allocator.hpp"
#include "containers/arena_mmr.hpp"
#include "containers/pmr.hpp"
#include "containers/mmr_allocator.hpp"
#include "containers/pmr_allocator.hpp"
#include "containers/buffer_pmr.hpp"
#include "containers/vector2.hpp"

#include "configure_warning_pragmas.hpp"

template <typename T>
using vec_pmr = ml::vector2<T, ml::pmr_allocator<T>>;

template <typename T, std::size_t CAPACITY>
using stack_pmr = ml::buffer_pmr<T, CAPACITY, ml::pmr>;

using intvec = vec_pmr<int>;

#ifndef DEBUG_ENABLED
#define SKIP_IF_RELEASE GTEST_SKIP() << "Not tested in release mode."
#else
#define SKIP_IF_RELEASE
#endif

/*
smr = stack memory resource
*/
// Construction
TEST(vector2, smr_init_empty) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
// Element access
TEST(vector2, smr_at_elem0) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    values.push_back(1);
    EXPECT_EQ(values.at(0), 1);
}
TEST(vector2, smr_at_oob) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_THROW(values.at(0), std::out_of_range);
}
TEST(vector2, smr_back_empty) {
    SKIP_IF_RELEASE;
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_THROW(values.back(), std::out_of_range);
}
TEST(vector2, smr_back) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    values.push_back(1);
    values.push_back(2);
    EXPECT_EQ(values.back(), 2);
}
TEST(vector2, smr_front_empty) {
    SKIP_IF_RELEASE;
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_THROW(values.front(), std::out_of_range);
}
TEST(vector2, smr_front) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    values.push_back(1);
    EXPECT_EQ(values.front(), 1);
}
TEST(vector2, smr_index_operator) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    values.push_back(1);
    EXPECT_EQ(values[0], 1);
}
TEST(vector2, smr_index_operator_oob) {
    SKIP_IF_RELEASE;
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_THROW(values[0], std::out_of_range);
}
TEST(vector2, smr_data_member_empty) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_EQ(nullptr, values.data());
}
TEST(vector2, smr_data_member) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    values.push_back(1);
    EXPECT_EQ(values.data(), &values.front());
}
// Iterators
TEST(vector2, smr_int_accumulate_fwd) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    static constexpr std::size_t n_elems{10};
    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }
    EXPECT_EQ(std::accumulate(values.begin(), values.end(), 0), 45);
}
TEST(vector2, smr_inc_accumulate_const_fwd) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    static constexpr std::size_t n_elems{10};
    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }
    EXPECT_EQ(std::accumulate(values.cbegin(), values.cend(), 0), 45);
}
TEST(vector2, smr_inc_accumulate_reverse) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    static constexpr std::size_t n_elems{10};
    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }
    EXPECT_EQ(std::accumulate(values.rbegin(), values.rend(), 0), 45);
}
TEST(vector2, smr_inc_accumulate_const_reverse) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    static constexpr std::size_t n_elems{10};
    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }
    EXPECT_EQ(std::accumulate(values.crbegin(), values.crend(), 0), 45);
}
// Capacity
TEST(vector2, smr_max_size_int) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_EQ(values.max_size(), std::numeric_limits<typename intvec::difference_type>::max() / sizeof(int));
}
// Modification
TEST(vector2, smr_clear_elems) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};
    static constexpr std::size_t n_elems{10};

    for (std::size_t i{0}; i < n_elems; ++i) {
        values.push_back(int(i));
    }
    EXPECT_EQ(values.size(), n_elems);
    EXPECT_FALSE(values.empty());

    values.clear();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(vector2, smr_emplace_back_struct_multiple) {
    struct TestStruct {
        int a;
        int b;
        TestStruct(int a_, int b_)
            : a(a_)
            , b(b_) {}
    };

    stack_pmr<TestStruct, 100> resource;
    vec_pmr<TestStruct> values{&resource};

    static constexpr std::size_t n_elems{10};

    for (int i{0}; i < static_cast<int>(n_elems); ++i) {
        values.emplace_back(i, i + 1);
    }

    EXPECT_EQ(values.size(), n_elems);
    EXPECT_FALSE(values.empty());
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
TEST(vector2, smr_pop_back_multiple_ints) {
    stack_pmr<int, 100> resource;
    intvec values{&resource};

    static constexpr std::size_t to_add{10};
    static constexpr std::size_t to_pop{2};

    for (std::size_t i{0}; i < to_add; ++i) {
        values.push_back(int(i));
    }

    EXPECT_EQ(values.size(), to_add);
    EXPECT_FALSE(values.empty());

    for (std::size_t i{0}; i < to_pop; ++i) {
        values.pop_back();
    }

    EXPECT_EQ(values.size(), to_add - to_pop);
    EXPECT_FALSE(values.empty());
}
TEST(vector2, smr_pop_back_empty) {
    SKIP_IF_RELEASE;

    stack_pmr<int, 100> resource;
    intvec values{&resource};
    EXPECT_TRUE(values.empty());
    EXPECT_THROW(values.pop_back(), std::out_of_range);
}
