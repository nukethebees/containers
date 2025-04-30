#include <gtest/gtest.h>

#include "containers/dlist.hpp"

TEST(dlist, empty) {
    ml::dlist<int> values;
    EXPECT_TRUE(values.empty());
}
