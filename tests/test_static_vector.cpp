#include <algorithm>
#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/static_vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(static_vector, init_empty_int_vector) {
    ml::static_vector<int, 10> values;
    EXPECT_EQ(values.capacity(), 10);
    EXPECT_EQ(values.size(), 0);
}