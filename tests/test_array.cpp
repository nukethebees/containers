#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "Containers/array.hpp"

TEST(array, init_empty_int_array) {
    constexpr std::size_t size{3};
    ml::array<int, size> values;
    ASSERT_EQ(values.size(), size);
}
TEST(array, init_int_array) {
    constexpr std::size_t size{3};
    ml::array<int, size> values{1, 2, 3};
    ASSERT_EQ(values.size(), size);
}
TEST(array, accumulate_ints) {
    ml::array<int, 3> values{1, 2, 3};

    auto sum{std::accumulate(values.begin(), values.end(), 0)};

    ASSERT_EQ(sum, 6);
}