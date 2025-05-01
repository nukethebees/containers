#include <algorithm>
#include <numeric>

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
TEST(dlist, push_front) {
    ml::dlist<int> values;
    values.push_front(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
TEST(dlist, push_front_and_back) {
    ml::dlist<int> values;

    values.push_front(1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);

    values.push_back(2);
    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 2);
}
TEST(dlist, pop_back) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);

    values.pop_back();
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);

    values.pop_back();
    EXPECT_TRUE(values.empty());
}
TEST(dlist, pop_front) {
    ml::dlist<int> values;

    values.push_back(1);
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);

    values.pop_front();
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 2);
    EXPECT_EQ(values.back(), 2);

    values.pop_front();
    EXPECT_TRUE(values.empty());
}
TEST(dlist, clear_3elem_list) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    EXPECT_EQ(values.size(), 3);
    values.clear();
    EXPECT_TRUE(values.empty());
}
TEST(dlist, range_based_for_loop_sum) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    int sum{0};
    for (auto const& value : values) {
        sum += value;
    }
    EXPECT_EQ(sum, 6);
}
TEST(dlist, accumulate) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    auto sum = std::accumulate(values.begin(), values.end(), 0);
    EXPECT_EQ(sum, 6);
}
TEST(dlist, reduce_find_max) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    auto max = std::reduce(values.begin(), values.end(), 0, [](int a, int b) { return std::max(a, b); });
    EXPECT_EQ(max, 3);
}
TEST(dlist, for_loop_sum_reverse) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    int sum{0};
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}
TEST(dlist, accumulate_reverse) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);

    auto sum = std::accumulate(values.rbegin(), values.rend(), 0);
    EXPECT_EQ(sum, 6);
}
TEST(dlist, reduce_reverse_find_max) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    values.push_back(3);
    auto max = std::reduce(values.rbegin(), values.rend(), 0, [](int a, int b) { return std::max(a, b); });
    EXPECT_EQ(max, 3);
}
TEST(dlist, emplace_front) {
    ml::dlist<int> values;
    values.emplace_front(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
TEST(dlist, emplace_back) {
    ml::dlist<int> values;
    values.emplace_back(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
