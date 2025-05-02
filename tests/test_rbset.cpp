#include <gtest/gtest.h>

#include "containers/rbset.hpp"

#include "configure_warning_pragmas.hpp"

TEST(rbset, empty_set) {
    ml::rbset<int> set;
    ASSERT_TRUE(set.empty());
}
