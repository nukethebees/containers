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

#include "sorting_tests.hpp"

#include "configure_warning_pragmas.hpp"

template <auto T>
struct SortingWrapper {
    template <typename... Args>
    static auto call(Args&&... args) {
        return T(std::forward<Args>(args)...);
    }
};

#define WRAP_FN(FN) SortingWrapper<[](auto&&... args) { return FN(std::forward<decltype(args)>(args)...); }>

using UnsignedSortingFns =
    ::testing::Types<WRAP_FN(ml::bubble_sort),
                     SortingWrapper<[](auto begin, auto end) -> void { ml::bucket_sort(begin, end, 10); }>,
                     WRAP_FN(ml::counting_sort),
                     WRAP_FN(ml::heap_sort),
                     WRAP_FN(ml::insertion_sort),
                     WRAP_FN(ml::merge_sort),
                     WRAP_FN(ml::quick_sort),
                     WRAP_FN(ml::radix_sort),
                     WRAP_FN(ml::selection_sort)>;
INSTANTIATE_TYPED_TEST_SUITE_P(My, UnsignedIntSortingTest, UnsignedSortingFns);

using SignedSortingFns = ::testing::Types<WRAP_FN(ml::bubble_sort),
                                          WRAP_FN(ml::heap_sort),
                                          WRAP_FN(ml::insertion_sort),
                                          WRAP_FN(ml::merge_sort),
                                          WRAP_FN(ml::quick_sort),
                                          WRAP_FN(ml::selection_sort)>;
INSTANTIATE_TYPED_TEST_SUITE_P(My, SignedIntSortingTest, SignedSortingFns);
