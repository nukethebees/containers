#include <vector>
#include <unordered_map>
#include <list>

#include <gtest/gtest.h>

#include "containers/arena_memory_resource.hpp"
#include "containers/polymorphic_allocator.hpp"

TEST(polymorphic_allocator, resource_allocate_bytes) {
    ml::ArenaMemoryResourcePmr resource;

    constexpr std::size_t size = 128;
    constexpr std::size_t alignment = alignof(std::max_align_t);

    void* ptr = resource.allocate(size, alignment);
    EXPECT_NE(ptr, nullptr);

    resource.deallocate(ptr, size, alignment);
}

TEST(polymorphic_allocator, resource_allocate_and_deallocate_multiple) {
    ml::ArenaMemoryResourcePmr resource;

    constexpr std::size_t size1 = 64;
    constexpr std::size_t size2 = 256;
    constexpr std::size_t alignment = alignof(std::max_align_t);

    void* ptr1 = resource.allocate(size1, alignment);
    void* ptr2 = resource.allocate(size2, alignment);

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2);

    resource.deallocate(ptr1, size1, alignment);
    resource.deallocate(ptr2, size2, alignment);
}

TEST(polymorphic_allocator, resource_alignment_check) {
    ml::ArenaMemoryResourcePmr resource;

    constexpr std::size_t size = 128;
    constexpr std::size_t alignment = 64;

    void* ptr = resource.allocate(size, alignment);
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignment, 0);

    resource.deallocate(ptr, size, alignment);
}
TEST(polymorphic_allocator, vector_basic_operations) {
    ml::ArenaMemoryResourcePmr resource;
    ml::polymorphic_allocator<int> alloc{&resource};
    std::vector<int, ml::polymorphic_allocator<int>> vec{alloc};

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}
TEST(polymorphic_allocator, vector_large_allocation) {
    ml::ArenaMemoryResourcePmr resource;
    ml::polymorphic_allocator<int> alloc{&resource};
    std::vector<int, ml::polymorphic_allocator<int>> vec{alloc};

    constexpr int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        vec.push_back(i);
    }

    EXPECT_EQ(vec.size(), num_elements);
    EXPECT_EQ(vec.front(), 0);
    EXPECT_EQ(vec.back(), num_elements - 1);
}

TEST(polymorphic_allocator, vector_copy_and_shared_resource) {
    ml::ArenaMemoryResourcePmr resource;
    ml::polymorphic_allocator<int> alloc{&resource};
    std::vector<int, ml::polymorphic_allocator<int>> vec1{alloc};

    // Fill the vector
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);

    // Copy the vector
    std::vector<int, ml::polymorphic_allocator<int>> vec2 = vec1;

    // Ensure both vectors are valid and have the same content
    EXPECT_EQ(vec1.size(), 3);
    EXPECT_EQ(vec2.size(), 3);
    EXPECT_EQ(vec1[0], 1);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec1[1], 2);
    EXPECT_EQ(vec2[1], 2);
    EXPECT_EQ(vec1[2], 3);
    EXPECT_EQ(vec2[2], 3);

    // Ensure both vectors use the same memory resource
    EXPECT_EQ(vec1.get_allocator().resource(), vec2.get_allocator().resource());
}

TEST(polymorphic_allocator, list_basic_operations) {
    ml::ArenaMemoryResourcePmr resource;
    ml::polymorphic_allocator<int> alloc{&resource};
    std::list<int, ml::polymorphic_allocator<int>> lst{alloc};

    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);

    EXPECT_EQ(lst.size(), 3);
    auto it = lst.begin();
    EXPECT_EQ(*it++, 1);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it++, 3);
}
TEST(polymorphic_allocator, list_large_allocation) {
    ml::ArenaMemoryResourcePmr resource;
    ml::polymorphic_allocator<int> alloc{&resource};
    std::list<int, ml::polymorphic_allocator<int>> lst{alloc};

    constexpr int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        lst.push_back(i);
    }

    EXPECT_EQ(lst.size(), num_elements);
    EXPECT_EQ(lst.front(), 0);
    EXPECT_EQ(lst.back(), num_elements - 1);
}

using int_map_alloc = ml::polymorphic_allocator<std::pair<int const, int>>;
using int_map = std::unordered_map<int, int, std::hash<int>, std::equal_to<>, int_map_alloc>;

TEST(polymorphic_allocator, unordered_map_basic_operations) {
    ml::ArenaMemoryResourcePmr resource;
    int_map_alloc alloc{&resource};
    int_map map{alloc};

    map[1] = 10;
    map[2] = 20;
    map[3] = 30;

    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map[1], 10);
    EXPECT_EQ(map[2], 20);
    EXPECT_EQ(map[3], 30);
}
TEST(polymorphic_allocator, unordered_map_large_allocation) {
    ml::ArenaMemoryResourcePmr resource;
    int_map_alloc alloc{&resource};
    int_map map{alloc};

    constexpr int num_elements = 10000;
    for (int i = 0; i < num_elements; ++i) {
        map[i] = i * 10;
    }

    EXPECT_EQ(map.size(), num_elements);
    EXPECT_EQ(map[0], 0);
    EXPECT_EQ(map[num_elements - 1], (num_elements - 1) * 10);
}
