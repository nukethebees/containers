#include <algorithm>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include "containers/bst.hpp"

#include "configure_warning_pragmas.hpp"

TEST(bst, empty_bst) {
    ml::bst<int> bst;
    ASSERT_TRUE(bst.empty());
}
TEST(bst, insert_single) {
    ml::bst<int> bst;
    bst.insert(1);
    ASSERT_EQ(bst.size(), 1);
}
TEST(bst, remove_single_verify_null_root) {
    ml::bst<int> bst;
    bst.insert(1);
    bst.clear();
    ASSERT_EQ(nullptr, bst.root_value());
}
TEST(bst, contains_int1) {
    ml::bst<int> bst;
    bst.insert(1);
    ASSERT_TRUE(bst.contains(1));
}
TEST(bst, contains_string_foo) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    ASSERT_TRUE(bst.contains("foo"));
}
TEST(bst, string_contains_lv_const_ptr) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    char const* str{"foo"};
    ASSERT_TRUE(bst.contains(str));
}
TEST(bst, string_contains_rv_const_ptr) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    ASSERT_TRUE(bst.contains("foo"));
}
TEST(bst, remove_three) {
    ml::bst<int> bst;
    bst.insert(1);

    bst.insert(5);
    bst.insert(4);
    bst.insert(6);

    EXPECT_EQ(bst.size(), 4);

    bst.remove_from(5);
    EXPECT_EQ(bst.size(), 1);
}
TEST(bst, get_max) {
    ml::bst<int> bst;
    bst.insert(1);
    bst.insert(5);
    bst.insert(4);
    bst.insert(6);

    EXPECT_EQ(*bst.max(), 6);
}
TEST(bst, get_min) {
    ml::bst<int> bst;
    bst.insert(1);
    bst.insert(5);
    bst.insert(4);
    bst.insert(6);

    EXPECT_EQ(*bst.min(), 1);
}
TEST(bst, range_based_for_sum) {
    ml::bst<int> bst;

    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }

    auto sum = 0;
    auto end{bst.end()};
    for (auto it{bst.begin()}; it != end; ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 15);
}
TEST(bst, accumulate) {
    ml::bst<int> bst;
    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }
    auto sum = std::accumulate(bst.begin(), bst.end(), 0);
    EXPECT_EQ(sum, 15);
}
TEST(bst, accumulate_const) {
    ml::bst<int> bst;
    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }
    auto sum = std::accumulate(bst.cbegin(), bst.cend(), 0);
    EXPECT_EQ(sum, 15);
}
TEST(bst, range_based_for_sum_reverse) {
    ml::bst<int> bst;

    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }

    auto sum = 0;
    auto end{bst.crend()};
    for (auto it{bst.crbegin()}; it != end; ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, 15);
}
TEST(bst, accumulate_reverse) {
    ml::bst<int> bst;
    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }
    auto sum = std::accumulate(bst.rbegin(), bst.rend(), 0);
    EXPECT_EQ(sum, 15);
}
TEST(bst, accumulate_reverse_const) {
    ml::bst<int> bst;
    std::vector<int> values{1, 2, 3, 4, 5};
    for (auto const& value : values) {
        bst.insert(value);
    }
    auto sum = std::accumulate(bst.crbegin(), bst.crend(), 0);
    EXPECT_EQ(sum, 15);
}
