#include <memory_resource>

#include <gtest/gtest.h>

#include "containers/stack_pmr.hpp"

#include "configure_warning_pragmas.hpp"

TEST(stack_pmr, resource_init) {
    ml::stack_pmr stack;
    ASSERT_EQ(stack.capacity(), 0);
}
