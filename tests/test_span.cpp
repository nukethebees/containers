#include <array>
#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "Containers/span.hpp"

using size_t = ml::span<int>::size_t;

TEST(span, ptr_size_ctor) {
    int * ptr{nullptr};
    constexpr size_t size{1};
    auto span{ml::span(ptr, size)};
}
TEST(span, from_array) {
    std::array<int, 5> values{0};
    auto span{ml::span(values)};
}
TEST(span, const_from_array) {
    std::array<int, 5> values{0};
    auto span{ml::span<int const>(values)};
}
TEST(span, foreach_from_array) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};

    int sum{0};
    for (auto val : span) {
        sum += val;
    }

    ASSERT_EQ(sum, 6);
}
TEST(span, accumulate_from_array) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};

    auto sum{std::accumulate(span.begin(), span.end(), 0)};

    ASSERT_EQ(sum, 6);
}