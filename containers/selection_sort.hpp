#pragma once

#include <iterator>
#include <utility>

namespace ml {
template <typename Iterator>
    requires std::bidirectional_iterator<Iterator>
void selection_sort(Iterator begin, Iterator end) {
    Iterator smallest{begin};

    for (auto iter_outer{begin}; iter_outer != end; ++iter_outer) {
        smallest = iter_outer;

        for (auto iter_inner{std::next(smallest)}; iter_inner != end; ++iter_inner) {
            if (*iter_inner < *smallest) {
                smallest = iter_inner;
            }
        }

        std::swap(*smallest, *iter_outer);
    }
}
}
