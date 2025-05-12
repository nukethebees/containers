#include <gtest/gtest.h>

#include "containers/linked_vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(linked_vector, init_empty_int_vector) {
    ml::linked_vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, reserve_one_elem) {
    ml::linked_vector<int> values;
    values.reserve(1);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
