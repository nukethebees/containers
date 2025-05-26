#pragma once

#include <array>
#include <algorithm>
#include <format>
#include <type_traits>
#include <vector>

namespace ml {
template <typename Iterator>
    requires std::is_unsigned_v<typename Iterator::value_type>
void radix_sort(Iterator begin, Iterator end) {
    static constexpr std::size_t num_base{10};
    std::array<char, 100> buffer{};

    auto const max_value{*std::max_element(begin, end)};
    auto const num_digits{static_cast<std::size_t>(std::format_to(buffer.begin(), "{}", max_value) - buffer.begin())};

    using T = typename Iterator::value_type;
    using Bucket = std::vector<T>;
    std::array<Bucket, num_base> buckets;

    for (std::size_t digit{0}; digit < num_digits; ++digit) {
        for (auto it{begin}; it != end; ++it) {
            // Convert to a string
            auto const str{std::format_to(buffer.begin(), "{:040}", *it)};
            // Get the nth character (RHS == 0)
            char const c{*(str - 1 - digit)};
            // Convert to int
            int const bucket_idx{c - '0'};
            // Add the value to the respective bucket
            buckets[bucket_idx].push_back(*it);
        }

        // Empty the buckets back into the original iterators
        auto it{begin};
        for (auto& bucket : buckets) {
            for (auto val : bucket) {
                *it = val;
                ++it;
            }
            bucket.clear();
        }
    }
}
}
