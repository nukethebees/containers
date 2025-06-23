#include <memory_resource>

#include <gtest/gtest.h>

#include "containers/stack_pmr.hpp"

#include "configure_warning_pragmas.hpp"

TEST(stack_pmr, resource_init) {
    ml::stack_pmr stack;
    ASSERT_EQ(stack.capacity(), 0);
}
TEST(stack_pmr, reserve_buffer) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    ASSERT_EQ(stack.capacity(), 1 << 10);
}
TEST(stack_pmr, reserve_twice) {
    ml::stack_pmr stack;
    stack.reserve(1);
    ASSERT_THROW(stack.reserve(1), std::runtime_error);
}
