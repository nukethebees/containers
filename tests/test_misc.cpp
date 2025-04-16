#include <gtest/gtest.h>

#include "containers/misc.hpp"

TEST(max_fn, HandlesIntegers) {
    EXPECT_EQ(ml::max(3, 5), 5);
    EXPECT_EQ(ml::max(10, 2), 10);
}

TEST(max_fn, HandlesDoubles) {
    EXPECT_DOUBLE_EQ(ml::max(3.14, 2.71), 3.14);
    EXPECT_DOUBLE_EQ(ml::max(1.1, 1.9), 1.9);
}

TEST(types_to_index, FindsCorrectIndex) {
    using namespace ml;
    using indexer = types_to_index<int, double, char>;

    EXPECT_EQ(indexer::get<int>(), 0);
    EXPECT_EQ(indexer::get<double>(), 1);
    EXPECT_EQ(indexer::get<char>(), 2);
}

TEST(types_to_index, HandlesNonExistentType) {
    using namespace ml;
    types_to_index<int, double, char> indexer;

    // This test will fail to compile if uncommented, as expected.
    // EXPECT_EQ(indexer.get<float>(), -1);
}
