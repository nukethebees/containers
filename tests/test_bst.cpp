#include <string>

#include <gtest/gtest.h>

#include "containers/bst.hpp"

#include "configure_warning_pragmas.hpp"

TEST(bst, empty_bst) {
    ml::bst<int> bst;
    ASSERT_TRUE(bst.empty());
}
TEST(bst, insert_single) {
    ml::bst<int> bst;
    bst.insert(1);
    ASSERT_EQ(bst.size(), 1);
}
TEST(bst, remove_single_verify_null_root) {
    ml::bst<int> bst;
    bst.insert(1);
    bst.clear();
    ASSERT_EQ(nullptr, bst.root());
}
TEST(bst, contains_int1) {
    ml::bst<int> bst;
    bst.insert(1);
    ASSERT_TRUE(bst.contains(1));
}
TEST(bst, contains_string_foo) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    ASSERT_TRUE(bst.contains("foo"));
}
TEST(bst, string_contains_lv_const_ptr) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    char const* str{"foo"};
    ASSERT_TRUE(bst.contains(str));
}
TEST(bst, string_contains_rv_const_ptr) {
    ml::bst<std::string> bst;
    bst.insert("foo");
    ASSERT_TRUE(bst.contains("foo"));
}
