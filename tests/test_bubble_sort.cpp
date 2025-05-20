#include <vector>

#include "gtest/gtest.h"

#include "containers/bubble_sort.hpp"

TEST(bubble_sort, misc) {
    std::vector<int> v{5, 4, 3, 2, 1};
    ml::bubble_sort(v.begin(), v.end());
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}
