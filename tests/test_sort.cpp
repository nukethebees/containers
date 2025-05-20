#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "containers/bubble_sort.hpp"
#include "containers/selection_sort.hpp"

template <typename T>
class SortTest : public testing::Test {};

template <auto sort_fn>
void test_simple_int_sort() {
    std::array<int, 5> v{5, 4, 3, 2, 1};
    std::array<int, 5> v_exp{1, 2, 3, 4, 5};

    sort_fn(v.begin(), v.end());
    EXPECT_EQ(v, v_exp);
}

#define WRAP_FN(FN) [](auto&&... args) { return FN(std::forward<decltype(args)>(args)...); }

TEST(bubble_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::bubble_sort)>();
}
TEST(selection_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::selection_sort)>();
}