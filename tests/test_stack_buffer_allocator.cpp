#include <cstdint>
#include <memory_resource>
#include <print>

#include <gtest/gtest.h>

#include "containers/pmr.hpp"
#include "containers/mmr_allocator.hpp"
#include "containers/pmr_allocator.hpp"
#include "containers/buffer_mmr.hpp"
#include "containers/buffer_pmr.hpp"

#include "configure_warning_pragmas.hpp"

template <typename T, std::size_t CAPACITY>
struct VecConfig : ml::StackAllocConfig<T, CAPACITY> {
    using Resource = typename ml::StackAllocConfig<T, CAPACITY>::Resource;
    using Allocator = typename ml::StackAllocConfig<T, CAPACITY>::Allocator;
    using Vec = std::vector<T, Allocator>;
};

TEST(stack_buffer_allocator, resource_init) {
    ml::buffer_mmr<1024> resource;
    ASSERT_EQ(resource.remaining_capacity(), 1024);
}
TEST(stack_buffer_allocator, resource_allocate) {
    ml::buffer_mmr<1024> resource;
    auto* ptr{resource.allocate(16, alignof(int))};
    ASSERT_NE(ptr, nullptr);
    ASSERT_EQ(resource.remaining_capacity(), 1008);
}
TEST(stack_buffer_allocator, resource_allocate_overflow) {
    ml::buffer_mmr<1024> resource;
    ASSERT_THROW((void)resource.allocate(1025, alignof(int)), std::bad_alloc);
}

TEST(stack_buffer_allocator, vector_init) {
    static constexpr std::size_t n_ints{1024};
    using config = VecConfig<int, n_ints>;
    config::Resource resource;
    config::Allocator alloc{&resource};

    std::vector<int, config::Allocator> vec{alloc};
}
TEST(stack_buffer_allocator, vector_push_back) {
    static constexpr std::size_t n_ints{1024};
    using config = VecConfig<int, n_ints>;
    config::Resource resource;
    config::Allocator alloc{&resource};
    config::Vec vec{alloc};
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    ASSERT_EQ(vec.size(), 5);
}
TEST(stack_buffer_allocator, feed_two_vectors) {
    static constexpr std::size_t n_ints{1024};
    using config = VecConfig<int, n_ints>;
    config::Resource resource;
    config::Allocator alloc{&resource};
    config::Vec vec1{alloc};
    config::Vec vec2{alloc};
    vec1.push_back(1);
    vec2.push_back(2);
    ASSERT_EQ(vec1.size(), 1);
    ASSERT_EQ(vec2.size(), 1);
}
TEST(stack_buffer_allocator, vector_push_back_overflow) {
    static constexpr std::size_t n_ints{5};
    using config = VecConfig<int, n_ints>;
    config::Resource resource;
    config::Allocator alloc{&resource};
    config::Vec vec{alloc};

    int volatile i{0};
    while (i < 10) {
        try {
            vec.push_back(int{i});
        } catch (std::bad_alloc const&) {
            SUCCEED();
            return;
        }

        i = i + 1;
    }

    FAIL() << "Should have thrown an exception";
}

// std::pmr
TEST(stack_buffer_pmr_std, init_pmr) {
    ml::buffer_pmr<int, 1024, std::pmr::memory_resource> resource;
}
TEST(stack_buffer_pmr_std, sum_ints_in_vector) {
    ml::buffer_pmr<int, 1024, std::pmr::memory_resource> resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec{alloc};

    int sum{0};
    for (int i{0}; i < 10; ++i) {
        vec.push_back(i);
    }

    for (auto const& i : vec) {
        sum += i;
    }

    ASSERT_EQ(sum, 45);
}

// ml::pmr
TEST(buffer_pmr, init_pmr) {
    ml::buffer_pmr<int, 1024, ml::pmr> resource;
}
TEST(buffer_pmr, sum_ints_in_vector) {
    ml::buffer_pmr<int, 1024, ml::pmr> resource;
    ml::pmr_allocator<int> alloc{&resource};
    std::vector<int, ml::pmr_allocator<int>> vec{alloc};

    int sum{0};
    for (int i{0}; i < 10; ++i) {
        vec.push_back(i);
    }

    for (auto const& i : vec) {
        sum += i;
    }

    ASSERT_EQ(sum, 45);
}
TEST(buffer_pmr, extend_alloc) {
    static constexpr std::size_t old_elems{16};
    static constexpr std::size_t new_elems{32};
    std::size_t expected_size{new_elems * sizeof(int)};

    ml::buffer_pmr<int, 1024, ml::pmr> resource;
    ml::pmr_allocator<int> alloc{&resource};

    auto* ptr{alloc.allocate(old_elems)};
    ASSERT_NE(ptr, nullptr);

    auto* extended_ptr{alloc.extend(ptr, old_elems, new_elems)};
    EXPECT_EQ(extended_ptr, ptr);
    EXPECT_EQ(resource.remaining_capacity(), resource.capacity() - expected_size);
    EXPECT_EQ(resource.size(), expected_size);
}
TEST(buffer_pmr, extend_alloc_oob) {
    ml::buffer_pmr<int, 1024, ml::pmr> resource;
    ml::pmr_allocator<int> alloc{&resource};

    auto* ptr{alloc.allocate(16)};
    ASSERT_NE(ptr, nullptr);

    EXPECT_THROW(alloc.extend(ptr, 16, 320000000), std::bad_alloc);
}
