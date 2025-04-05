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

