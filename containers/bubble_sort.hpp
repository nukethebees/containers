#pragma once

#include <iterator>
#include <utility>

namespace ml {
template <typename Iterator>
    requires std::bidirectional_iterator<Iterator>
void bubble_sort(Iterator begin, Iterator end) {
    bool is_sorted = false;
    while (!is_sorted) {
        is_sorted = true;

        for (auto it = begin; it != std::prev(end); ++it) {
            auto next_it = std::next(it);

            if (*it > *next_it) {
                std::swap(*it, *next_it);
                is_sorted = false;
            }
        }
    }
}
}
