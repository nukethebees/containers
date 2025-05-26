#include <array>
#include <memory_resource>

#include "gtest/gtest.h"

#include "containers/binary_heap.hpp"

#include "configure_warning_pragmas.hpp"

TEST(binary_heap, init) {
    ml::binary_heap<int> heap;
}
TEST(binary_heap, insert_min_reverse) {
    std::array<int, 6> arr = {0, 1, 2, 3, 4, 5};
    ml::binary_heap<int> heap{std::pmr::new_delete_resource()};
    heap.insert(5);
    heap.insert(4);
    heap.insert(3);
    heap.insert(2);
    heap.insert(1);
    heap.insert(0);

    EXPECT_EQ(heap.size(), 6);
    for (std::size_t i{0}; i < 6; ++i) {
        auto popped{heap.pop()};
        auto exp{arr[i]};
        EXPECT_EQ(popped, exp);
    }
}
TEST(binary_heap, insert_max_reverse) {
    std::array<int, 6> arr = {5, 4, 3, 2, 1, 0};
    ml::binary_heap<int, std::greater<>> heap{std::pmr::new_delete_resource()};
    heap.insert(0);
    heap.insert(1);
    heap.insert(2);
    heap.insert(3);
    heap.insert(4);
    heap.insert(5);

    EXPECT_EQ(heap.size(), 6);
    for (std::size_t i{0}; i < 6; ++i) {
        auto popped{heap.pop()};
        auto exp{arr[i]};
        EXPECT_EQ(popped, exp);
    }
}
