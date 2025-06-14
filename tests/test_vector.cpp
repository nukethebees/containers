#include <algorithm>
#include <cstddef>
#include <numeric>
#include <tuple>

#include <gtest/gtest.h>

#include "test_container_single_elem_iterators.hpp"
#include "vector_container_traits.hpp"

#include "containers/vector.hpp"

#include "configure_warning_pragmas.hpp"

TEST(vector, init_empty_int_vector) {
    ml::vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
}
TEST(vector, emplace_single) {
    ml::vector<int> values;
    values.emplace_back(1);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, emplace_multiple) {
    ml::vector<int> values;
    for (int i = 0; i < 5; ++i) {
        values.emplace_back(i);
    }
    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[1], 1);
    EXPECT_EQ(values[2], 2);
    EXPECT_EQ(values[3], 3);
    EXPECT_EQ(values[4], 4);
}
TEST(vector, emplace_struct_three_members) {
    struct TestStruct {
        int a;
        double b;
        char c;
    };
    ml::vector<TestStruct> values;
    values.emplace_back(1, 2.0, 'a');
    EXPECT_EQ(values[0].a, 1);
    EXPECT_EQ(values[0].b, 2.0);
    EXPECT_EQ(values[0].c, 'a');
}
TEST(vector, push_back_int) {
    ml::vector<int> values;
    values.push_back(10);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, push_back_multiple) {
    ml::vector<int> values;
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    EXPECT_EQ(values.size(), 5);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[1], 1);
    EXPECT_EQ(values[2], 2);
    EXPECT_EQ(values[3], 3);
    EXPECT_EQ(values[4], 4);
}
TEST(vector, push_back_string) {
    ml::vector<std::string> values;
    values.push_back("Hello");
    EXPECT_EQ(values[0], "Hello");
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, reserve) {
    ml::vector<int> values;
    values.reserve(5);
    EXPECT_EQ(values.capacity(), 5);
}

TEST(vector, front) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.front(), 10);
}

TEST(vector, back) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.back(), 20);
}

TEST(vector, clear) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    values.clear();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}

TEST(vector, empty) {
    ml::vector<int> values;
    EXPECT_TRUE(values.empty());
    values.emplace_back(10);
    EXPECT_FALSE(values.empty());
}

TEST(vector, size) {
    ml::vector<int> values;
    EXPECT_EQ(values.size(), 0);
    values.emplace_back(10);
    EXPECT_EQ(values.size(), 1);
    values.emplace_back(20);
    EXPECT_EQ(values.size(), 2);
}

template <typename T, typename Allocator>
struct ContainerTestTraits<ml::vector<T, Allocator>> : vector_container_traits<ml::vector<T, Allocator>> {
    using Container = ml::vector<T, Allocator>;
    using vector_container_traits<Container>::add_element;
    using vector_container_traits<Container>::add_elements;
    using vector_container_traits<Container>::emplace_element;
};

using T0 = ml::vector<int, std::allocator<int>>;
using T1 = ml::vector<int, std::pmr::polymorphic_allocator<int>>;

using TestTypes = ::testing::Types<ContainerDefinition<T0, []() { return std::make_tuple(T0()); }>,
                                   ContainerDefinition<T1, []() { return std::make_tuple(T1()); }>>;
INSTANTIATE_TYPED_TEST_SUITE_P(vector_, ContainerSingleElemIteratorTest, TestTypes);
