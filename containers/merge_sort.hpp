#pragma once

#include <iterator>
#include <utility>
#include <vector>

#include <print>

namespace ml {
template <typename Iterator>
    requires std::bidirectional_iterator<Iterator>
void merge_sort(Iterator begin, Iterator end) {
    using T = typename Iterator::value_type;
    struct SplitVector {
        std::vector<T> left;
        std::vector<T> right;
    };
    static constexpr auto split_vector{[](auto it, auto end) {
        auto const n{static_cast<std::size_t>(end - it)};
        auto const mid{n / 2};
        auto const it_mid{(it + mid)};

        SplitVector split_vector;
        split_vector.left.reserve(mid);
        split_vector.right.reserve(n - mid);

        while (it != it_mid) {
            split_vector.left.push_back(*it);
            ++it;
        }
        while (it != end) {
            split_vector.right.push_back(*it);
            ++it;
        }

        return split_vector;
    }};
    static constexpr auto merge{[](auto it, auto end, auto itl, auto lend, auto itr, auto rend) {
        auto l_done{[&] { return (itl == lend); }};
        auto r_done{[&] { return (itr == rend); }};

        while (!l_done() && !r_done()) {
            if (*itl < *itr) {
                *it = *itl;
                itl++;
            } else {
                *it = *itr;
                itr++;
            }
            it++;
        }

        while (!l_done()) {
            *it = *itl;
            itl++;
            it++;
        }
        while (!r_done()) {
            *it = *itr;
            itr++;
            it++;
        }
    }};

    if ((end - begin) < 2) {
        return;
    }

    auto sv{split_vector(begin, end)};

    merge_sort(sv.left.begin(), sv.left.end());
    merge_sort(sv.right.begin(), sv.right.end());

    merge(begin, end, sv.left.begin(), sv.left.end(), sv.right.begin(), sv.right.end());
}
}
