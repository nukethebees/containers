#include <numeric>
#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "containers/span.hpp"

using size_t = typename ml::span<int>::size_type;

// span_iterator
TEST(span_iterator, indexing_from_array) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values).begin()};

    ASSERT_EQ(span[0], 1);
}

// span
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
TEST(span, indexing_from_array) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};

    ASSERT_EQ(span[0], 1);
}
TEST(span, indexing_from_array_const) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};
    ASSERT_EQ(span[0], 1);
}

TEST(span, begin) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(*span.begin(), 1);
}
TEST(span, begin_const) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};
    ASSERT_EQ(*span.begin(), 1);
}
TEST(span, cbegin) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(*span.cbegin(), 1);
}
TEST(span, cbegin_const) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};
    ASSERT_EQ(*span.cbegin(), 1);
}
TEST(span, end) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(*(span.end() - 1), 3);
}
TEST(span, end_const) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};
    ASSERT_EQ(*(span.end() - 1), 3);
}
TEST(span, cend) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(*(span.cend() - 1), 3);
}
TEST(span, cend_const) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int const>(values)};
    ASSERT_EQ(*(span.cend() - 1), 3);
}

TEST(span, front) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(span.front(), 1);
}
TEST(span, back) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(span.back(), 3);
}

TEST(span, at) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(span.at(0), 1);
}
TEST(span, at_out_of_bounds) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_THROW(span.at(4), std::out_of_range);
}

TEST(span, size) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(span.size(), 3);
}
TEST(span, size_bytes) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_EQ(span.size_bytes(), sizeof(int) * 3);
}
TEST(span, empty) {
    std::array<int, 3> values{{1, 2, 3}};
    auto span{ml::span<int>(values)};
    ASSERT_FALSE(span.empty());
}
TEST(span, empty_zero) {
    std::array<int, 0> values{};
    auto span{ml::span<int>(values)};
    ASSERT_TRUE(span.empty());
}