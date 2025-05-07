#include <algorithm>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include "containers/bst.hpp"

#include "configure_warning_pragmas.hpp"

static std::vector<int> values1{1, 2, 3, 4, 5};
static auto sum1{std::accumulate(values1.begin(), values1.end(), 0)};

static std::vector<int> values2{50, 55, 60, 35, 25, 20, 5, 15, 22, 57, 59};
static auto sum2{std::accumulate(values2.begin(), values2.end(), 0)};

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
TEST(bst, range_based_for_sum_2) {
    ml::bst<int> bst;

    for (auto const& value : values2) {
        bst.insert(value);
    }

    auto sum = 0;
    auto end{bst.end()};
    for (auto it{bst.begin()}; it != end; ++it) {
        sum += *it;
    }

    EXPECT_EQ(sum, sum2);
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
TEST(bst, accumulate_complicated_tree) {
    ml::bst<int> bst;

    auto sum{0};
    auto expected_sum{0};

    for (auto const& value : values2) {
        bst.insert(value);
        expected_sum += value;
    }

    sum = std::accumulate(bst.begin(), bst.end(), 0);

    EXPECT_EQ(sum, expected_sum);
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
TEST(bst, range_based_for_sum_reverse1) {
    ml::bst<int> bst;

    for (auto const& value : values1) {
        bst.insert(value);
    }

    auto sum = 0;
    auto end{bst.crend()};
    for (auto it{bst.crbegin()}; it != end; ++it) {
        auto val{*it};
        sum += val;
    }

    EXPECT_EQ(sum, sum1);
}
TEST(bst, range_based_for_sum_reverse2) {
    ml::bst<int> bst;

    for (auto const& value : values2) {
        bst.insert(value);
    }

    auto sum = 0;
    auto end{bst.crend()};
    for (auto it{bst.crbegin()}; it != end; ++it) {
        auto val{*it};
        sum += val;
    }

    EXPECT_EQ(sum, sum2);
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

TEST(bst, iterator_forward_order) {
    ml::bst<int> bst;
    for (auto const& value : values2) {
        bst.insert(value);
    }

    auto b_it{bst.cbegin()};
    auto b_end{bst.cend()};

    auto vals{values2};
    std::sort(vals.begin(), vals.end(), std::less<int>());
    auto v_it{vals.cbegin()};

    for (; b_it != b_end; ++b_it, ++v_it) {
        EXPECT_EQ(*b_it, *v_it);
    }
}
TEST(bst, iterator_reverse_order) {
    ml::bst<int> bst;
    for (auto const& value : values2) {
        bst.insert(value);
    }

    auto b_it{bst.crbegin()};
    auto b_end{bst.crend()};

    auto vals{values2};
    std::sort(vals.begin(), vals.end(), std::less<int>());
    auto v_it{vals.crbegin()};

    for (; b_it != b_end; ++b_it, ++v_it) {
        EXPECT_EQ(*b_it, *v_it);
    }
}
