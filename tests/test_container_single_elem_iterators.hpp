#pragma once

#include <utility>

#include "gtest/gtest.h"

#include "container_definition.hpp"
#include "container_test_traits.hpp"

template <typename ContainerDefinition>
class ContainerSingleElemIteratorTest : public testing::Test {
  public:
    using Traits = ContainerTestTraits<typename ContainerDefinition::Container>;
    auto construct() -> decltype(auto) { return ContainerDefinition::construct(); }
};
TYPED_TEST_SUITE_P(ContainerSingleElemIteratorTest);

TYPED_TEST_P(ContainerSingleElemIteratorTest, accumulate_ints) {
    using T = typename TestFixture::Traits;

    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum{std::accumulate(values.begin(), values.end(), 0)};
    EXPECT_EQ(sum, 6);
}
TYPED_TEST_P(ContainerSingleElemIteratorTest, accumulate_ints_const) {
    using T = typename TestFixture::Traits;

    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum{std::accumulate(values.cbegin(), values.cend(), 0)};
    EXPECT_EQ(sum, 6);
}
TYPED_TEST_P(ContainerSingleElemIteratorTest, accumulate_ints_reverse) {
    using T = typename TestFixture::Traits;

    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum{std::accumulate(values.rbegin(), values.rend(), 0)};
    EXPECT_EQ(sum, 6);
}
TYPED_TEST_P(ContainerSingleElemIteratorTest, accumulate_ints_const_reverse) {
    using T = typename TestFixture::Traits;

    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum{std::accumulate(values.crbegin(), values.crend(), 0)};
    EXPECT_EQ(sum, 6);
}
TYPED_TEST_P(ContainerSingleElemIteratorTest, for_sum_reverse) {
    using T = typename TestFixture::Traits;
    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum = 0;
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}
TYPED_TEST_P(ContainerSingleElemIteratorTest, range_based_for) {
    using T = typename TestFixture::Traits;
    auto initialised_container{this->construct()};
    auto& values{std::get<0>(initialised_container)};
    T::add_elements(values, 1, 2, 3);

    int sum = 0;
    for (auto const& value : values) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}

REGISTER_TYPED_TEST_SUITE_P(ContainerSingleElemIteratorTest,
                            accumulate_ints,
                            accumulate_ints_const,
                            accumulate_ints_reverse,
                            accumulate_ints_const_reverse,
                            for_sum_reverse,
                            range_based_for);
