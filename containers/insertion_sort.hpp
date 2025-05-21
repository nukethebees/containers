#pragma once

#include <iterator>
#include <utility>

namespace ml {
template <typename Iterator>
    requires std::bidirectional_iterator<Iterator>
void insertion_sort(Iterator begin, Iterator end) {
    // Go through each element of the container
    // Assume the first element is sorted
    for (auto it_to_sort{std::next(begin)}; it_to_sort != end; ++it_to_sort) {
        // Go through the sorted values from right to left and place the new value in its correct place
        auto it_lr{it_to_sort};
        while (it_lr != begin) {
            auto prev{it_lr - 1};
            if (*it_lr > *prev) {
                break;
            }
            std::swap(*it_lr, *prev);
            --it_lr;
        }
    }
}
}

