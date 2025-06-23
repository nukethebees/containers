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
    auto frame(stack.create_frame());
    ASSERT_NE(frame.get(), nullptr);
}

TEST(stack_pmr, vector_ints) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    ASSERT_EQ(0, stack.size());

    {
        auto frame(stack.create_frame());
        ASSERT_EQ(sizeof(ml::stack_pmr_frame), stack.size());

        std::pmr::vector<int> vec{frame.get()};
        for (int i{0}; i < 5; ++i) {
            vec.push_back(i);
            ASSERT_EQ(i, vec.back());
        }
    }

    ASSERT_EQ(0, stack.size());
}
TEST(stack_pmr, three_frames) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    ASSERT_EQ(0, stack.size());

    {
        auto frame0(stack.create_frame());
        ASSERT_EQ(sizeof(ml::stack_pmr_frame), stack.size());
        {
            auto frame1(stack.create_frame());
            ASSERT_EQ(sizeof(ml::stack_pmr_frame) * 2, stack.size());
            {
                auto frame2(stack.create_frame());
                ASSERT_EQ(sizeof(ml::stack_pmr_frame) * 3, stack.size());
            }
        }
    }

    ASSERT_EQ(0, stack.size());
}
TEST(stack_pmr, older_frame_try_alloc) {
    ml::stack_pmr stack;
    stack.reserve(1 << 10);
    {
        auto frame0(stack.create_frame());
        {
            auto frame1(stack.create_frame());
            ASSERT_THROW((void)frame0->allocate(8, 8), std::runtime_error);
        }
    }
}
