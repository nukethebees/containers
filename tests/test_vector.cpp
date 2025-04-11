#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/vector.hpp"

TEST(vector, init_empty_int_vector) {
    ml::vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
}
TEST(vector, emplace_single) {
    ml::vector<int> values;
    values.emplace_back(1);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, reserve) {
    ml::vector<int> values;
    values.reserve(5);
    EXPECT_EQ(values.capacity(), 5);
}