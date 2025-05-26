#include <array>
#include <vector>

#include "gtest/gtest.h"

#include "containers/bubble_sort.hpp"
#include "containers/bucket_sort.hpp"
#include "containers/counting_sort.hpp"
#include "containers/heap_sort.hpp"
#include "containers/insertion_sort.hpp"
#include "containers/merge_sort.hpp"
#include "containers/quick_sort.hpp"
#include "containers/radix_sort.hpp"
#include "containers/selection_sort.hpp"

#include "configure_warning_pragmas.hpp"

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

static constexpr std::array<unsigned int, 20> v6i{22, 10, 11, 27, 67, 44, 76, 22, 74, 22,
                                                  84, 16, 20, 98, 38, 41, 8,  41, 75, 18};
static constexpr std::array<unsigned int, 20> v6o{8,  10, 11, 16, 18, 20, 22, 22, 22, 27,
                                                  38, 41, 41, 44, 67, 74, 75, 76, 84, 98};

static constexpr std::array<unsigned int, 200> v7i{
    63,       22272336, 14924,    83968,    19458808, 88488,    7003329,  387,     49711,    47,       79189,
    39935,    64,       518269,   9742817,  42374114, 5451428,  63885886, 35349,   725444,   515431,   65025290,
    9722277,  44679803, 2761,     6049,     66430,    99895,    30777791, 975,     729,      521,      9723,
    67,       5967192,  59390,    2572,     83196655, 34863758, 9,        519,     2,        30,       4664,
    866469,   48639,    85,       6977184,  8262468,  98,       4159341,  3,       8453609,  64174,    49,
    48692978, 8312937,  5090,     4133,     58,       311966,   17,       4642194, 680,      286337,   6497131,
    450488,   44,       8940619,  1915,     5797157,  74738267, 39542775, 89446,   7355,     87,       49,
    8016745,  5531900,  61,       6,        4539742,  1,        5840,     732243,  94930911, 6583,     398,
    519,      8357826,  61882,    34,       41839173, 473466,   11,       2,       62169515, 4219,     70147196,
    72066,    892130,   651,      81548098, 4926979,  441,      82336,    3022764, 2866412,  9120,     30366,
    938,      44400,    93310596, 91996,    9468,     2056,     6,        96446,   620,      20,       714,
    43114313, 62,       69245,    802,      6091,     19757727, 46,       16,      9356698,  2562,     53566,
    3803840,  9440,     504943,   64144,    3636258,  72575535, 8,        30,      873,      10228221, 31,
    60595592, 48,       98298748, 22009,    1129,     203021,   43335,    780865,  4425,     5766,     89545,
    19890,    20091,    86430532, 4359,     3679,     103069,   41114266, 818,     45908,    69055535, 1,
    2283675,  446420,   780,      9989377,  36960131, 961,      5,        29146,   55270681, 46578245, 276444,
    147,      4181495,  93098229, 7082836,  9545,     36,       5,        15,      6760,     65403450, 9,
    2,        66713329, 98689492, 38838769, 37118743, 661,      88736461, 21778,   8943,     679784,   5857521,
    4814,     85858};
static constexpr std::array<unsigned int, 200> v7o{
    1,        1,        2,        2,        2,        3,        5,        5,        6,        6,        8,
    9,        9,        11,       15,       16,       17,       20,       30,       30,       31,       34,
    36,       44,       46,       47,       48,       49,       49,       58,       61,       62,       63,
    64,       67,       85,       87,       98,       147,      387,      398,      441,      519,      519,
    521,      620,      651,      661,      680,      714,      729,      780,      802,      818,      873,
    938,      961,      975,      1129,     1915,     2056,     2562,     2572,     2761,     3679,     4133,
    4219,     4359,     4425,     4664,     4814,     5090,     5766,     5840,     6049,     6091,     6583,
    6760,     7355,     8943,     9120,     9440,     9468,     9545,     9723,     14924,    19890,    20091,
    21778,    22009,    29146,    30366,    35349,    39935,    43335,    44400,    45908,    48639,    49711,
    53566,    59390,    61882,    64144,    64174,    66430,    69245,    72066,    79189,    82336,    83968,
    85858,    88488,    89446,    89545,    91996,    96446,    99895,    103069,   203021,   276444,   286337,
    311966,   446420,   450488,   473466,   504943,   515431,   518269,   679784,   725444,   732243,   780865,
    866469,   892130,   2283675,  2866412,  3022764,  3636258,  3803840,  4159341,  4181495,  4539742,  4642194,
    4926979,  5451428,  5531900,  5797157,  5857521,  5967192,  6497131,  6977184,  7003329,  7082836,  8016745,
    8262468,  8312937,  8357826,  8453609,  8940619,  9356698,  9722277,  9742817,  9989377,  10228221, 19458808,
    19757727, 22272336, 30777791, 34863758, 36960131, 37118743, 38838769, 39542775, 41114266, 41839173, 42374114,
    43114313, 44679803, 46578245, 48692978, 55270681, 60595592, 62169515, 63885886, 65025290, 65403450, 66713329,
    69055535, 70147196, 72575535, 74738267, 81548098, 83196655, 86430532, 88736461, 93098229, 93310596, 94930911,
    98298748, 98689492};

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
TEST(bucket_sort, simple_uints) {
    auto input{v6i};
    ml::bucket_sort(input.begin(), input.end(), 10);
    EXPECT_EQ(input, v6o);
}
TEST(counting_sort, simple_uints) {
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
TEST(radix_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::radix_sort), v7i, v7o>();
}
TEST(selection_sort, simple_ints) {
    test_simple_int_sort<WRAP_FN(ml::selection_sort), v1i, v1o>();
}
