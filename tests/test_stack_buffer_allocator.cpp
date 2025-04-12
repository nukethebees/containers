#include <cstdint>

#include <gtest/gtest.h>

#include "containers/stack_buffer_allocator.hpp"
#include "containers/memory_resource_allocator.hpp"

template <typename T, std::size_t CAPACITY>
using StackResource = ml::StackBufferMemoryResource<CAPACITY * sizeof(T)>;

template <typename T, std::size_t CAPACITY>
using StackAlloc = ml::MemoryResourceAllocator<T, StackResource<T, CAPACITY>>;

TEST(stack_buffer_allocator, resource_init) {
    ml::StackBufferMemoryResource<1024> resource;
    ASSERT_EQ(resource.remaining_capacity(), 1024);
}
TEST(stack_buffer_allocator, resource_allocate) {
    ml::StackBufferMemoryResource<1024> resource;
    auto * ptr{resource.allocate(16, alignof(int))};
    ASSERT_NE(ptr, nullptr);
    ASSERT_EQ(resource.remaining_capacity(), 1008);
}
TEST(stack_buffer_allocator, resource_allocate_overflow) {
    ml::StackBufferMemoryResource<1024> resource;
    ASSERT_THROW((void)resource.allocate(1025, alignof(int)), std::bad_alloc);
}

TEST(stack_buffer_allocator, vector_init) {
    static constexpr std::size_t n_ints{1024};
    StackResource<int, n_ints> resource;
    StackAlloc<int, n_ints> alloc{&resource};

    std::vector<int, StackAlloc<int, n_ints>> vec{alloc};
}
TEST(stack_buffer_allocator, vector_push_back) {
    static constexpr std::size_t n_ints{1024};
    StackResource<int, n_ints> resource;
    StackAlloc<int, n_ints> alloc{&resource};
    std::vector<int, StackAlloc<int, n_ints>> vec{alloc};
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    ASSERT_EQ(vec.size(), 5);
}
TEST(stack_buffer_allocator, feed_two_vectors) {
    static constexpr std::size_t n_ints{1024};
    StackResource<int, n_ints> resource;
    StackAlloc<int, n_ints> alloc{&resource};
    std::vector<int, StackAlloc<int, n_ints>> vec1{alloc};
    std::vector<int, StackAlloc<int, n_ints>> vec2{alloc};
    vec1.push_back(1);
    vec2.push_back(2);
    ASSERT_EQ(vec1.size(), 1);
    ASSERT_EQ(vec2.size(), 1);
}
TEST(stack_buffer_allocator, vector_push_back_overflow) {
    static constexpr std::size_t n_ints{5};
    StackResource<int, n_ints> resource;
    StackAlloc<int, n_ints> alloc{&resource};
    std::vector<int, StackAlloc<int, n_ints>> vec{alloc};
    int i{0};
    while (resource.remaining_capacity() >= sizeof(int)) {
        vec.push_back(i);
        i++;
    }
    ASSERT_THROW(vec.push_back(i), std::bad_alloc);
}
