#pragma once

#include <iterator>

namespace ml {
template <typename Iterator>
    requires std::bidirectional_iterator<Iterator>
void selection_sort(Iterator begin, Iterator end) {}
}
