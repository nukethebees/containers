#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/array.hpp"

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
TEST(array, constexpr_indexing) {
    constexpr std::size_t size{3};
    constexpr ml::array<int, size> values{1, 2, 3};
    constexpr auto val{values[0]};

    ASSERT_EQ(val, 1);
}