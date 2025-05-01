#include <cstdint>
#include <print>

#include <gtest/gtest.h>

#include "containers/stack_buffer_memory_resource.hpp"
#include "containers/memory_resource_allocator.hpp"

#include "configure_warning_pragmas.hpp"

template <typename T, std::size_t CAPACITY>
struct VecConfig : ml::StackAllocConfig<T, CAPACITY> {
    using Resource = typename ml::StackAllocConfig<T, CAPACITY>::Resource;
    using Allocator = typename ml::StackAllocConfig<T, CAPACITY>::Allocator;
    using Vec = std::vector<T, Allocator>;
};

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

    volatile int i{0};
    while (i < 10) {
        try {
            vec.push_back(int{i});
        }
        catch (std::bad_alloc) {
            SUCCEED();
            return;
        }
        
        i = i + 1;
    }
    
    FAIL() << "Should have thrown an exception";
}
