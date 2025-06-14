#pragma once

// Iterator tests are handled in test_container_single_elem_iterators

#include <utility>

#include "gtest/gtest.h"

#include "container_definition.hpp"
#include "container_test_traits.hpp"

template <typename ContainerDefinition>
class VectorTest : public testing::Test {
  public:
    using Traits = ContainerTestTraits<typename ContainerDefinition::Container>;
    auto construct() -> decltype(auto) { return ContainerDefinition::construct(); }
};
TYPED_TEST_SUITE_P(VectorTest);

#define INIT_CONTAINER(CONTAINER_NAME)             \
    auto initialised_container{this->construct()}; \
    auto& CONTAINER_NAME{std::get<0>(initialised_container)};

// Simple construction
TYPED_TEST_P(VectorTest, construct_empty) {
    INIT_CONTAINER(container);
    EXPECT_EQ(container.size(), 0);
    EXPECT_TRUE(container.empty());
}
TYPED_TEST_P(VectorTest, emplace_single) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::emplace_element(container, 1);
    EXPECT_EQ(container.size(), 1);
    EXPECT_FALSE(container.empty());
}
TYPED_TEST_P(VectorTest, add_multiple) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.size(), 3);
    EXPECT_FALSE(container.empty());

    EXPECT_EQ(container[0], 1);
    EXPECT_EQ(container[1], 2);
    EXPECT_EQ(container[2], 3);
}
TYPED_TEST_P(VectorTest, front) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.front(), 1);
}
TYPED_TEST_P(VectorTest, back) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.back(), 3);
}
TYPED_TEST_P(VectorTest, clear) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.size(), 3);
    container.clear();
    EXPECT_EQ(container.size(), 0);
}
TYPED_TEST_P(VectorTest, at) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.at(0), 1);
    EXPECT_EQ(container.at(1), 2);
}
TYPED_TEST_P(VectorTest, indexing_operator) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container[0], 1);
    EXPECT_EQ(container[1], 2);
}
TYPED_TEST_P(VectorTest, data_member) {
    using T = typename TestFixture::Traits;
    INIT_CONTAINER(container);
    T::add_elements(container, 1, 2, 3);
    EXPECT_EQ(container.data(), &container.front());
}

REGISTER_TYPED_TEST_SUITE_P(VectorTest,
                            construct_empty,
                            emplace_single,
                            add_multiple,
                            front,
                            back,
                            clear,
                            at,
                            indexing_operator,
                            data_member);
