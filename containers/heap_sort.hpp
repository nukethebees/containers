#pragma once

#include <iterator>
#include <utility>

#include "binary_heap.hpp"

namespace ml {
template <typename Iterator>
void heap_sort(Iterator begin, Iterator end) {
    binary_heap<typename Iterator::value_type> heap;
    heap.reserve(static_cast<std::size_t>(end - begin));

    for (auto it{begin}; it != end; ++it) {
        heap.insert(*it);
    }

    while (!heap.empty()) {
        *begin = heap.pop();
        ++begin;
    }
}
}
