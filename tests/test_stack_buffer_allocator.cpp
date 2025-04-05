#include <cstdint>

#include <gtest/gtest.h>

#include "Containers/stack_buffer_allocator.hpp"

template <typename T, std::size_t capacity>
using stack_vector = std::vector<T, ml::stack_buffer_allocator<T, capacity>>;

template <std::size_t capacity>
using int_vec = stack_vector<int, capacity>;


TEST(stack_buffer_allocator, vector_init) {
    int_vec<5> vec;
}

TEST(stack_buffer_allocator, vector_push_back) {
    int_vec<5> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    ASSERT_EQ(vec.size(), 5);
}

TEST(stack_buffer_allocator, vector_push_back_overflow) {
    int_vec<5> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    ASSERT_THROW(vec.push_back(6), std::bad_alloc);
}