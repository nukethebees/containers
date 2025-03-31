#include <array>
#include <cstddef>

#include <gtest/gtest.h>

#include "Containers/span.hpp"

using size_t = ml::span<int>::size_t;

TEST(span, ptr_size_ctor) {
    int * ptr{nullptr};
    constexpr size_t size{1};

    auto span{ml::span(ptr, size)};

    ASSERT_EQ(size, span.size());
}
TEST(span, from_array) {
    std::array<int, 5> values{0};
    
    auto span{ml::span(values)};
}
TEST(span, const_from_array) {
    std::array<int, 5> values{0};

    auto span{ml::span<int const>(values)};
}