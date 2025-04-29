#include <gtest/gtest.h>

#include "containers/slist.hpp"

TEST(slist, empty) {
    ml::slist<int> list;
    ASSERT_EQ(list.size(), 0);
}
TEST(slist, add_elem) {
    ml::slist<int> list;
    list.emplace_back(1);
    ASSERT_EQ(list.back(), 1);
}
TEST(slist, add_two_elems) {
    ml::slist<int> list;
    list.emplace_back(1);
    list.emplace_back(2);

    ASSERT_EQ(list.size(), 2);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 2);
}
