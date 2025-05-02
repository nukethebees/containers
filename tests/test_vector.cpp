#include <algorithm>
#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(vector, init_empty_int_vector) {
    ml::vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
}
TEST(vector, emplace_single) {
    ml::vector<int> values;
    values.emplace_back(1);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, emplace_multiple) {
    ml::vector<int> values;
    for (int i = 0; i < 5; ++i) {
        values.emplace_back(i);
    }
    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[1], 1);
    EXPECT_EQ(values[2], 2);
    EXPECT_EQ(values[3], 3);
    EXPECT_EQ(values[4], 4);
}
TEST(vector, emplace_struct_three_members) {
    struct TestStruct {
        int a;
        double b;
        char c;
    };
    ml::vector<TestStruct> values;
    values.emplace_back(1, 2.0, 'a');
    EXPECT_EQ(values[0].a, 1);
    EXPECT_EQ(values[0].b, 2.0);
    EXPECT_EQ(values[0].c, 'a');
}
TEST(vector, push_back_int) {
    ml::vector<int> values;
    values.push_back(10);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, push_back_multiple) {
    ml::vector<int> values;
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[1], 1);
    EXPECT_EQ(values[2], 2);
    EXPECT_EQ(values[3], 3);
    EXPECT_EQ(values[4], 4);
}
TEST(vector, push_back_string) {
    ml::vector<std::string> values;
    values.push_back("Hello");
    EXPECT_EQ(values[0], "Hello");
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, reserve) {
    ml::vector<int> values;
    values.reserve(5);
    EXPECT_EQ(values.capacity(), 5);
}

TEST(vector, front) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.front(), 10);
}

TEST(vector, back) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.back(), 20);
}

TEST(vector, clear) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    values.clear();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}

TEST(vector, empty) {
    ml::vector<int> values;
    EXPECT_TRUE(values.empty());
    values.emplace_back(10);
    EXPECT_FALSE(values.empty());
}

TEST(vector, size) {
    ml::vector<int> values;
    EXPECT_EQ(values.size(), 0);
    values.emplace_back(10);
    EXPECT_EQ(values.size(), 1);
    values.emplace_back(20);
    EXPECT_EQ(values.size(), 2);
}
TEST(vector, range_based_for) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);

    int sum = 0;
    for (auto const& value : values) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}
TEST(vector, accumulate) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);
    int sum = std::accumulate(values.begin(), values.end(), 0);
    EXPECT_EQ(sum, 6);
}
TEST(vector, accumulate_const) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);
    int sum = std::accumulate(values.cbegin(), values.cend(), 0);
    EXPECT_EQ(sum, 6);
}
TEST(vector, for_sum_reverse) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);
    int sum = 0;
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}
TEST(vector, accumulate_reverse) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);
    int sum = std::accumulate(values.rbegin(), values.rend(), 0);
    EXPECT_EQ(sum, 6);
}
TEST(vector, accumulate_const_reverse) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);
    int sum = std::accumulate(values.crbegin(), values.crend(), 0);
    EXPECT_EQ(sum, 6);
}

TEST(vector, pmr_allocator) {
    ml::vector<int, std::pmr::polymorphic_allocator<int>> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);

    int sum = 0;
    for (auto const& value : values) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}
