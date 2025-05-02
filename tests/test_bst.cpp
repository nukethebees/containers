#include <gtest/gtest.h>

#include "containers/bst.hpp"

#include "configure_warning_pragmas.hpp"

TEST(bst, empty_bst) {
    ml::bst<int> bst;
    ASSERT_TRUE(bst.empty());
}
