#pragma once

#include <type_traits>
#include <vector>

namespace ml {
template <typename Iterator>
    requires std::is_unsigned_v<typename Iterator::value_type>
void counting_sort(Iterator begin, Iterator end) {
    using T = typename Iterator::value_type;

    auto const n{end - begin};
    if (n < 2) {
        return;
    }

    auto max_value{*begin};
    for (auto it{std::next(begin)}; it != end; ++it) {
        if (*it > max_value) {
            max_value = *it;
        }
    }

    // Allocate the count array
    // The value is used as an index so we need max_value+1 elements
    auto const n_counts{static_cast<std::size_t>(max_value + 1)};
    std::vector<T> counts(n_counts, 0);

    // Count the elements
    for (auto it{begin}; it != end; ++it) {
        ++counts[*it];
    }

    // Reconstruct the original array
    for (std::size_t i{0}; i < n_counts; ++i) {
        auto count{counts[i]};
        if (count) {
            for (std::size_t j{0}; j < count; ++j) {
                *begin = static_cast<T>(i);
                ++begin;
            }
        }
    }
}
}
