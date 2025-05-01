#include <gtest/gtest.h>

#include "containers/dlist.hpp"

#include "configure_warning_pragmas.hpp"

TEST(dlist, empty) {
    ml::dlist<int> values;
    EXPECT_TRUE(values.empty());
}
TEST(dlist, push_back) {
    ml::dlist<int> values;
    values.push_back(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
