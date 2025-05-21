#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "containers/bubble_sort.hpp"
#include "containers/insertion_sort.hpp"
#include "containers/merge_sort.hpp"
#include "containers/selection_sort.hpp"

template <typename T>
class SortTest : public testing::Test {};

template <auto sort_fn>
void test_simple_int_sort() {
    std::array<int, 20> v{22, 10, 11, 27, 67, 44, 76, 22, 74, 22, 84, 16, 20, 98, 38, 41, 8, 41, 75, 18};
    std::array<int, 20> v_exp{8, 10, 11, 16, 18, 20, 22, 22, 22, 27, 38, 41, 41, 44, 67, 74, 75, 76, 84, 98};

    sort_fn(v.begin(), v.end());
    EXPECT_EQ(v, v_exp);
}

#define WRAP_FN(FN) [](auto&&... args) { return FN(std::forward<decltype(args)>(args)...); }

TEST(bubble_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::bubble_sort)>();
}
TEST(insertion_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::insertion_sort)>();
}
TEST(merge_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::merge_sort)>();
}
TEST(selection_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::selection_sort)>();
}
