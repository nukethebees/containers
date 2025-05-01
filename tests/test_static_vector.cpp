#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include "containers/static_vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(static_vector, init_empty_int_vector) {
    ml::static_vector<int, 10> values;
    EXPECT_EQ(values.capacity(), 10);
    EXPECT_EQ(values.size(), 0);
}
TEST(static_vector, push_back_int) {
    ml::static_vector<int, 10> values;
    values.push_back(10);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values.size(), 1);
}
TEST(static_vector, push_back_string) {
    ml::static_vector<std::string, 10> values{};
    values.push_back("Hello");
    EXPECT_EQ(values[0], "Hello");
    EXPECT_EQ(values.size(), 1);
}
TEST(static_vector, at_oob) {
    ml::static_vector<int, 10> values;
    values.push_back(10);
    EXPECT_THROW(values.at(1), std::out_of_range);
}
TEST(static_vector, emplace_back_string) {
    ml::static_vector<std::string, 10> values{};
    values.emplace_back("Hello");
    EXPECT_EQ(values[0], "Hello");
    EXPECT_EQ(values.size(), 1);
}
TEST(static_vector, pop_back_string) {
    ml::static_vector<std::string, 10> values{};
    values.emplace_back("Hello");
    values.pop_back();
    EXPECT_EQ(values.size(), 0);
}
TEST(static_vector, accumulate_int) {
    ml::static_vector<int, 10> values{};
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    auto sum = std::accumulate(values.begin(), values.end(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(static_vector, accumulate_const_int) {
    ml::static_vector<int, 10> values{};
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    auto sum = std::accumulate(values.cbegin(), values.cend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(static_vector, accumulate_reverse_int) {
    ml::static_vector<int, 10> values{};
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    auto sum = std::accumulate(values.rbegin(), values.rend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(static_vector, accumulate_const_reverse_int) {
    ml::static_vector<int, 10> values{};
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    auto sum = std::accumulate(values.crbegin(), values.crend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(static_vector, clear_strings) {
    ml::static_vector<std::string, 10> values{};
    values.emplace_back("Hello");
    values.emplace_back("World");
    EXPECT_EQ(values.size(), 2);
    values.clear();
    EXPECT_EQ(values.size(), 0);
}
