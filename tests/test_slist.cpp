#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/slist.hpp"

#include "configure_warning_pragmas.hpp"

TEST(slist, empty) {
    ml::slist<int> list;
    ASSERT_EQ(list.size(), 0);
}
TEST(slist, add_elem) {
    ml::slist<int> list;
    list.emplace_back(1);
    ASSERT_EQ(list.back(), 1);
}
TEST(slist, add_two_elems) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);

    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 2);
}
TEST(slist, pop_back) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.pop_back();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 1);
}
TEST(slist, pop_front) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.pop_front();
    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 2);
    ASSERT_EQ(list.back(), 2);
}
TEST(slist, emplace_front) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_front(2);
    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 2);
    ASSERT_EQ(list.back(), 1);
}
TEST(slist, push_back) {
    ml::slist<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 3);
}
TEST(slist, push_front) {
    ml::slist<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 3);
    ASSERT_EQ(list.back(), 1);
}
TEST(slist, clear) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.clear();
    ASSERT_EQ(list.size(), 0);
}
TEST(slist, insert_0) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.insert(0, 0);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 0);
    ASSERT_EQ(list.back(), 2);
}
TEST(slist, insert_end) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.insert(3, 2);
    ASSERT_EQ(list.size(), 3);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 3);
}
TEST(slist, range_based_for_loop_sum) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    int sum{0};
    for (auto const& elem : list) {
        sum += elem;
    }
    ASSERT_EQ(sum, 6);
}
TEST(slist, accumulate) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    auto sum = std::accumulate(list.begin(), list.end(), 0);
    ASSERT_EQ(sum, 6);
}
TEST(slist, reduce_find_max) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    auto max = std::reduce(list.begin(), list.end(), 0, [](int a, int b) { return std::max(a, b); });
    ASSERT_EQ(max, 3);
}
TEST(slist, map_double_values) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    std::transform(list.begin(), list.end(), list.begin(), [](int a) { return a * 2; });
    ASSERT_EQ(list.front(), 2);
    ASSERT_EQ(list.back(), 6);
}
