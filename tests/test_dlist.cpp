#include <gtest/gtest.h>

#include "containers/dlist.hpp"

#include "configure_warning_pragmas.hpp"

TEST(dlist, empty) {
    ml::dlist<int> values;
    EXPECT_TRUE(values.empty());
}
TEST(dlist, push_back) {
    ml::dlist<int> values;
    values.push_back(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
TEST(dlist, push_front) {
    ml::dlist<int> values;
    values.push_front(1);
    EXPECT_FALSE(values.empty());
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
}
TEST(dlist, push_front_and_back) {
    ml::dlist<int> values;
    
    values.push_front(1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);
    
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 2);
}
TEST(dlist, pop_back) {
    ml::dlist<int> values;
    values.push_back(1);
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);

    values.pop_back();
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 1);
    EXPECT_EQ(values.back(), 1);

    values.pop_back();
    EXPECT_TRUE(values.empty());
}
TEST(dlist, pop_front) {
    ml::dlist<int> values;

    values.push_back(1);
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);
    
    values.pop_front();
    EXPECT_EQ(values.size(), 1);
    EXPECT_EQ(values.front(), 2);
    EXPECT_EQ(values.back(), 2);
    
    values.pop_front();
    EXPECT_TRUE(values.empty());
}
