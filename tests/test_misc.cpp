#include <gtest/gtest.h>

#include "containers/misc.hpp"

#include "configure_warning_pragmas.hpp"

TEST(max_fn, handles_ints) {
    EXPECT_EQ(ml::max(3, 5), 5);
    EXPECT_EQ(ml::max(10, 2), 10);
}

TEST(max_fn, handles_doubles) {
    EXPECT_DOUBLE_EQ(ml::max(3.14, 2.71), 3.14);
    EXPECT_DOUBLE_EQ(ml::max(1.1, 1.9), 1.9);
}

TEST(types_to_index, finds_correct_index) {
    using namespace ml;
    using indexer = types_to_index<int, double, char>;

    EXPECT_EQ(indexer::get<int>(), 0);
    EXPECT_EQ(indexer::get<double>(), 1);
    EXPECT_EQ(indexer::get<char>(), 2);
}
TEST(types_to_index, size) {
    using namespace ml;
    using indexer = types_to_index<int, double, char>;

    EXPECT_EQ(3, indexer::size());
}
