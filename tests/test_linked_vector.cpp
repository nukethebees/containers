#include <string>

#include <gtest/gtest.h>

#include "containers/linked_vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(linked_vector, init_empty_int_vector) {
    ml::linked_vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, reserve_one_elem) {
    ml::linked_vector<int> values;
    values.reserve(1);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, push_back_int) {
    ml::linked_vector<int> values;
    values.reserve(1);
    values.push_back(42);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(linked_vector, clear_int) {
    ml::linked_vector<int> values;

    values.reserve(1);
    values.push_back(42);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());

    values.clear();
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, push_back_string) {
    ml::linked_vector<std::string> values;
    values.reserve(1);
    values.push_back("Hello");
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(linked_vector, emplace_string) {
    ml::linked_vector<std::string> values;
    values.reserve(1);
    values.emplace_back("Hello");
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
