#include <cstddef>
#include <vector>
#include <format>

#include <gtest/gtest.h>

#include "containers/arena_mmr_allocator.hpp"
#include "containers/arena_pmr.hpp"

#include "configure_warning_pragmas.hpp"

TEST(arena, empty_memory_resource) {
    ml::arena_mmr resource;
    // No pools should exist initially
    ASSERT_EQ(resource.pool(), nullptr);
}
TEST(arena, resource_request_bytes) {
    constexpr std::size_t size{100};

    ml::arena_mmr resource;
    auto* ptr = resource.allocate(size * sizeof(std::byte), alignof(std::byte));
    EXPECT_NE(ptr, nullptr);
}
TEST(arena, resource_emplace_and_use_value) {
    constexpr std::size_t value{42};

    ml::arena_mmr resource;
    auto* ptr = resource.allocate(sizeof(std::size_t), alignof(std::size_t));
    auto* emplaced_value = new (ptr) std::size_t(value);

    EXPECT_EQ(*emplaced_value, value);
}
TEST(arena, create_arena_and_allocate_int) {
    constexpr int value{15};

    ml::arena_mmr resource;
    auto* ptr = resource.allocate(sizeof(int), alignof(int));
    EXPECT_NE(ptr, nullptr);

    auto* emplaced_value = new (ptr) int(value);
    EXPECT_EQ(*emplaced_value, value);
}
TEST(arena, allocate_and_deallocate) {
    ml::arena_mmr resource;

    auto* ptr = resource.allocate(10 * sizeof(char), alignof(char));
    EXPECT_NE(ptr, nullptr);

    resource.deallocate(ptr, 10 * sizeof(char), alignof(char));
    // Deallocate should not throw or cause issues
}
TEST(arena, allocate_multiple_pools) {
    ml::arena_mmr resource;

    std::size_t large_size = resource.initial_capacity() * 2;
    auto* ptr1 = resource.allocate(resource.initial_capacity(), alignof(std::byte));
    auto* ptr2 = resource.allocate(large_size, alignof(std::byte));

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2);
}
TEST(arena, allocate_with_alignment) {
    ml::arena_mmr resource;

    constexpr std::size_t alignment = 64;
    auto* ptr = resource.allocate(128, alignment);

    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignment, 0);
}
TEST(arena, reallocate_within_pool) {
    ml::arena_mmr resource;

    auto* ptr = resource.allocate(10, alignof(std::byte));
    auto* ptr2 = resource.allocate(20, alignof(std::byte));

    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(ptr2, nullptr);
}
TEST(arena, allocate_large_object) {
    ml::arena_mmr resource;

    std::size_t large_size = resource.initial_capacity() * 10;
    auto* ptr = resource.allocate(large_size, alignof(std::byte));

    EXPECT_NE(ptr, nullptr);
}
TEST(arena, count_number_of_pools) {
    ml::arena_mmr resource;

    EXPECT_EQ(resource.n_pools(), 0); // Initially, no pools should exist.

    resource.allocate(resource.initial_capacity(), alignof(std::byte));
    EXPECT_EQ(resource.n_pools(), 1); // After first allocation, one pool should exist.

    resource.allocate(resource.initial_capacity() * 2, alignof(std::byte));
    EXPECT_EQ(resource.n_pools(), 2); // After a larger allocation, a second pool should be created.
}
TEST(arena, total_size_and_add_objects) {
    ml::arena_mmr resource;

    EXPECT_EQ(resource.total_size(), 0); // Initially, total size should be 0.

    // Allocate objects to create multiple pools
    std::size_t initial_capacity = resource.initial_capacity();
    resource.allocate(initial_capacity, alignof(std::byte)); // First pool
    EXPECT_EQ(resource.n_pools(), 1);
    EXPECT_EQ(resource.total_size(), initial_capacity);

    resource.allocate(initial_capacity * 2, alignof(std::byte)); // Second pool
    EXPECT_EQ(resource.n_pools(), 2);
    EXPECT_EQ(resource.total_size(), initial_capacity * 3);

    resource.allocate(initial_capacity * 4, alignof(std::byte)); // Third pool
    EXPECT_EQ(resource.n_pools(), 3);
    EXPECT_EQ(resource.total_size(), initial_capacity * 7);

    // Add objects to each pool
    auto* ptr1 = resource.allocate(sizeof(int), alignof(int));
    auto* ptr2 = resource.allocate(sizeof(double), alignof(double));
    auto* ptr3 = resource.allocate(sizeof(char), alignof(char));

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr3, nullptr);
}

// Realloc
TEST(arena, extend) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};

    auto const sz{sizeof(int)};
    auto const ao{alignof(int)};

    auto* ptr1{alloc.allocate_bytes(sz, ao)};
    EXPECT_EQ(resource.total_size(), sz);

    auto* ptr2{alloc.extend_bytes(ptr1, sz, sz * 2, ao)};
    EXPECT_EQ(resource.total_size(), sz * 2);

    EXPECT_EQ(ptr1, ptr2);
}

// Vector usage
TEST(arena, vector_basic_operations) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};
    std::vector<int, ml::arena_mmr_allocator<int>> vec{alloc};

    // Push back elements
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}
TEST(arena, vector_reserve_and_emplace) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};
    std::vector<int, ml::arena_mmr_allocator<int>> vec{alloc};

    // Reserve space and emplace elements
    vec.reserve(10);
    vec.emplace_back(10);
    vec.emplace_back(20);

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}
TEST(arena, vector_resize_and_access) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};
    std::vector<int, ml::arena_mmr_allocator<int>> vec{alloc};

    // Resize and access elements
    static constexpr std::size_t size{5};
    vec.resize(size, 42);

    EXPECT_EQ(vec.size(), size);
    for (std::size_t i{0}; i < size; ++i) {
        EXPECT_EQ(vec[i], 42);
    }
}
TEST(arena, vector_large_allocation) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};
    std::vector<int, ml::arena_mmr_allocator<int>> vec{alloc};

    // Add a large number of elements
    constexpr int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        vec.push_back(i);
    }

    EXPECT_EQ(vec.size(), num_elements);
    EXPECT_EQ(vec.front(), 0);
    EXPECT_EQ(vec.back(), num_elements - 1);
}
TEST(arena, vector_copy_and_move) {
    ml::arena_mmr resource;
    ml::arena_mmr_allocator<int> alloc{&resource};
    std::vector<int, ml::arena_mmr_allocator<int>> vec1{alloc};

    vec1.push_back(1);
    vec1.push_back(2);

    // Copy vector
    std::vector<int, ml::arena_mmr_allocator<int>> vec2 = vec1;
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);

    // Move vector
    std::vector<int, ml::arena_mmr_allocator<int>> vec3 = std::move(vec1);
    EXPECT_EQ(vec3.size(), 2);
    EXPECT_EQ(vec3[0], 1);
    EXPECT_EQ(vec3[1], 2);
    EXPECT_TRUE(vec1.empty());
}

// PMR
TEST(arena_pmr, empty_memory_resource) {
    ml::arena_pmr resource;
    SUCCEED();
}
TEST(arena_pmr, resource_request_bytes) {
    constexpr std::size_t size{100};

    ml::arena_pmr resource;
    auto* ptr = resource.allocate(size * sizeof(std::byte), alignof(std::byte));
    EXPECT_NE(ptr, nullptr);
    resource.deallocate(ptr, size * sizeof(std::byte), alignof(std::byte));
}
TEST(arena_pmr, allocate_with_alignment) {
    ml::arena_pmr resource;

    constexpr std::size_t alignment = 64;
    auto* ptr = resource.allocate(128, alignment);

    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignment, 0);

    resource.deallocate(ptr, 128, alignment);
}
TEST(arena_pmr, allocate_and_deallocate) {
    ml::arena_pmr resource;

    auto* ptr = resource.allocate(10 * sizeof(char), alignof(char));
    EXPECT_NE(ptr, nullptr);

    resource.deallocate(ptr, 10 * sizeof(char), alignof(char));
    // Deallocate should not throw or cause issues
}
TEST(arena_pmr, allocate_large_object) {
    ml::arena_pmr resource;

    std::size_t large_size = 1024 * 10;
    auto* ptr = resource.allocate(large_size, alignof(std::byte));

    EXPECT_NE(ptr, nullptr);
    resource.deallocate(ptr, large_size, alignof(std::byte));
}
TEST(arena_pmr, pmr_vector_basic_operations) {
    ml::arena_pmr resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec{alloc};

    // Push back elements
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}
TEST(arena_pmr, pmr_vector_reserve_and_emplace) {
    ml::arena_pmr resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec{alloc};

    // Reserve space and emplace elements
    vec.reserve(10);
    vec.emplace_back(10);
    vec.emplace_back(20);

    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec[0], 10);
    EXPECT_EQ(vec[1], 20);
}
TEST(arena_pmr, pmr_vector_resize_and_access) {
    ml::arena_pmr resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec{alloc};

    // Resize and access elements
    vec.resize(5, 42);

    EXPECT_EQ(vec.size(), 5);
    for (std::size_t i{0}; i < 5; ++i) {
        EXPECT_EQ(vec[i], 42);
    }
}
TEST(arena_pmr, pmr_vector_large_allocation) {
    ml::arena_pmr resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec{alloc};

    // Add a large number of elements
    constexpr int num_elements = 10000;
    for (int i{0}; i < num_elements; ++i) {
        vec.push_back(i);
    }

    EXPECT_EQ(vec.size(), num_elements);
    EXPECT_EQ(vec.front(), 0);
    EXPECT_EQ(vec.back(), num_elements - 1);
}
TEST(arena_pmr, pmr_vector_copy_and_move) {
    ml::arena_pmr resource;
    std::pmr::polymorphic_allocator<int> alloc{&resource};
    std::pmr::vector<int> vec1{alloc};

    vec1.push_back(1);
    vec1.push_back(2);

    // Copy vector
    std::pmr::vector<int> vec2 = vec1;
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);

    // Move vector
    std::pmr::vector<int> vec3 = std::move(vec1);
    EXPECT_EQ(vec3.size(), 2);
    EXPECT_EQ(vec3[0], 1);
    EXPECT_EQ(vec3[1], 2);
    EXPECT_TRUE(vec1.empty());
}
