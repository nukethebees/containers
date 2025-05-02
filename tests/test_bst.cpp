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
