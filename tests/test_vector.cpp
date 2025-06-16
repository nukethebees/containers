#include <algorithm>
#include <cstddef>
#include <numeric>
#include <tuple>

#include <gtest/gtest.h>

#include "test_container_single_elem_iterators.hpp"
#include "test_vector.hpp"
#include "vector_container_traits.hpp"

#include "containers/vector.hpp"
#include "containers/pmr_allocator.hpp"
#include "containers/new_delete_pmr.hpp"
#include "containers/buffer_pmr.hpp"

#include "configure_warning_pragmas.hpp"

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

template <typename T, typename Allocator>
struct ContainerTestTraits<ml::vector<T, Allocator>>
    : vector_container_traits<ml::vector<T, Allocator>> {
    using Container = ml::vector<T, Allocator>;
    using vector_container_traits<Container>::add_element;
    using vector_container_traits<Container>::add_elements;
    using vector_container_traits<Container>::emplace_element;
};

using T0 = ml::vector<int, std::allocator<int>>;
using TT0 = ContainerDefinition<T0, []() { return std::make_tuple(T0()); }>;
using T1 = ml::vector<int, std::pmr::polymorphic_allocator<int>>;
using TT1 = ContainerDefinition<T1, []() { return std::make_tuple(T1()); }>;

using T2 = ml::vector<int, ml::pmr_allocator<int>>;
using TT2 = ContainerDefinition<T2, []() {
    return std::make_tuple(T2(ml::get_new_delete_pmr<ml::pmr>()));
}>;

using T3 = ml::vector<int, ml::pmr_allocator<int>>;
using TT3 = ContainerDefinition<T3, []() {
    using Resource = ml::buffer_pmr<int, 1000, ml::pmr>;
    auto resource{std::make_unique<Resource>()};
    auto values{T3{resource.get()}};

    return std::make_tuple(std::move(values), std::move(resource));
}>;

using TestTypes = ::testing::Types<TT0, TT1, TT2, TT3>;
INSTANTIATE_TYPED_TEST_SUITE_P(vector_, ContainerSingleElemIteratorTest, TestTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(vector_, VectorTest, TestTypes);
