#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "containers/bubble_sort.hpp"
#include "containers/counting_sort.hpp"
#include "containers/heap_sort.hpp"
#include "containers/insertion_sort.hpp"
#include "containers/merge_sort.hpp"
#include "containers/quick_sort.hpp"
#include "containers/selection_sort.hpp"

template <typename T>
class SortTest : public testing::Test {};

static constexpr std::array<int, 48> v0i{-7, 12, -1, 0,  5,  -9, 8,  -3,  21, -5, 11, -2, 15, 1,  0,  -6,
                                         7,  -4, 19, -8, -1, 6,  3,  -10, 20, 9,  -7, 2,  0,  -1, 4,  1,
                                         13, -3, 16, 14, -2, 18, 22, -4,  10, -5, 17, -6, -9, -8, 11, 3};
static constexpr std::array<int, 48> v0o{-10, -9, -9, -8, -8, -7, -7, -6, -6, -5, -5, -4, -4, -3, -3, -2,
                                         -2,  -1, -1, -1, 0,  0,  0,  1,  1,  2,  3,  3,  4,  5,  6,  7,
                                         8,   9,  10, 11, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};

static constexpr std::array<int, 10> v1i{1, 2, 1, 1, 0, 0, 0, -1, -1, -1};
static constexpr std::array<int, 10> v1o{-1, -1, -1, 0, 0, 0, 1, 1, 1, 2};

static constexpr std::array<int, 20> v2i{22, 10, 11, 27, 67, 44, 76, 22, 74, 22, 84, 16, 20, 98, 38, 41, 8, 41, 75, 18};
static constexpr std::array<int, 20> v2o{8, 10, 11, 16, 18, 20, 22, 22, 22, 27, 38, 41, 41, 44, 67, 74, 75, 76, 84, 98};

static constexpr std::array<int, 8> v3i{42, 7, 19, 84, 3, 56, 29, 11};
static constexpr std::array<int, 8> v3o{3, 7, 11, 19, 29, 42, 56, 84};

static constexpr std::array<int, 5> v4i{10, 80, 30, 90, 40};
static constexpr std::array<int, 5> v4o{10, 30, 40, 80, 90};

static constexpr std::array<unsigned int, 5> v5i{10, 80, 30, 90, 40};
static constexpr std::array<unsigned int, 5> v5o{10, 30, 40, 80, 90};

template <auto sort_fn, std::array vi, std::array vo>
void test_simple_int_sort() {
    auto input{vi};

    sort_fn(input.begin(), input.end());
    EXPECT_EQ(input, vo);
}

#define WRAP_FN(FN) [](auto&&... args) { return FN(std::forward<decltype(args)>(args)...); }

TEST(bubble_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::bubble_sort), v1i, v1o>();
}
TEST(counting_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::counting_sort), v5i, v5o>();
}
TEST(heap_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::heap_sort), v1i, v1o>();
}
TEST(insertion_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::insertion_sort), v1i, v1o>();
}
TEST(merge_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::merge_sort), v1i, v1o>();
}
TEST(quick_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::quick_sort), v1i, v1o>();
}
TEST(selection_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::selection_sort), v1i, v1o>();
}
