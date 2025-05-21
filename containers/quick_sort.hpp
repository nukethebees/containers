#pragma once

#include <iterator>
#include <utility>

namespace ml {
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end) {
    auto const n{end - begin};
    if (n < 2) {
        return;
    }

    static constexpr auto partition{[](auto begin, auto end) -> auto {
        auto pivot{std::prev(end)};
        auto it_lt_pivot{begin};
        auto it{begin};

        // Move everything below the pivot
        while (it != pivot) {
            if (*it < *pivot) {
                std::swap(*it_lt_pivot, *it);
                ++it_lt_pivot;
            }

            ++it;
        }

        // Move pivot next to first number below it
        std::swap(*pivot, *it_lt_pivot);

        return it_lt_pivot;
    }};

    auto p{partition(begin, end)};
    // Sort up to but not including the pivot
    quick_sort(begin, p);
    // Sort after the pivot
    quick_sort(p + 1, end);
}
}
