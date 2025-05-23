#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <vector>

#include "insertion_sort.hpp"

namespace ml {
template <typename Iterator>
    requires std::is_unsigned_v<typename Iterator::value_type>
void bucket_sort(Iterator begin, Iterator end, std::size_t n_buckets) {
    using T = std::remove_cvref_t<decltype(*begin)>;
    using Bucket = std::vector<T>;
    using Buckets = std::vector<Bucket>;
    Buckets buckets(n_buckets);

    auto const max_value{*std::max_element(begin, end)};
    auto const norm{max_value + 1};

    for (auto it{begin}; it != end; ++it) {
        auto const index{*it * static_cast<T>(n_buckets) / norm};
        buckets[index].push_back(*it);
    }

    for (auto& bucket : buckets) {
        if (bucket.size() > 1) {
            insertion_sort(bucket.begin(), bucket.end());
        }
    }

    // Merge the sorted buckets back into the original range
    for (auto& bucket : buckets) {
        for (auto& elem : bucket) {
            *begin = elem;
            ++begin;
        }
    }
}
}
