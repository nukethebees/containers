#include <memory_resource>
#include <vector>

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

TEST(stack_pmr, create_frame) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    auto* frame(stack.create_frame());
    ASSERT_NE(frame, nullptr);
}

TEST(stack_pmr, vector_ints) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    auto* frame(stack.create_frame());

    std::pmr::vector<int> vec{frame};
    for (int i{0}; i < 5; ++i) {
        vec.push_back(i);
        ASSERT_EQ(i, vec.back());
    }
}
