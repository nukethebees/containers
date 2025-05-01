#include <cstddef>
#include <vector>
#include <gtest/gtest.h>

#include "containers/multi_arena_resource.hpp"

#include "configure_warning_pragmas.hpp"

TEST(multi_arena, create_with_multiple_resources) {
    constexpr std::size_t n_resources = 3;
    constexpr std::size_t initial_capacity = 1024;

    ml::MultiArenaMemoryResourcePmr resource{n_resources, initial_capacity};

    for (std::size_t i = 0; i < n_resources; ++i) {
        auto* arena = resource.get_resource(i);
        EXPECT_NE(arena, nullptr);
    }
}
TEST(multi_arena, allocate_from_specific_resource) {
    constexpr std::size_t n_resources = 2;
    constexpr std::size_t initial_capacity = 512;

    ml::MultiArenaMemoryResourcePmr resource{n_resources, initial_capacity};

    auto* arena = resource.get_resource(0);
    ASSERT_NE(arena, nullptr);

    auto* ptr = arena->allocate(128, alignof(std::byte));
    EXPECT_NE(ptr, nullptr);

    arena->deallocate(ptr, 128, alignof(std::byte));
}
TEST(multi_arena, allocate_across_multiple_resources) {
    constexpr std::size_t n_resources = 2;
    constexpr std::size_t initial_capacity = 256;

    ml::MultiArenaMemoryResourcePmr resource{n_resources, initial_capacity};

    for (std::size_t i = 0; i < n_resources; ++i) {
        auto* arena = resource.get_resource(i);
        ASSERT_NE(arena, nullptr);

        auto* ptr = arena->allocate(initial_capacity / 2, alignof(std::byte));
        EXPECT_NE(ptr, nullptr);

        arena->deallocate(ptr, initial_capacity / 2, alignof(std::byte));
    }
}
TEST(multi_arena, invalid_resource_index) {
    constexpr std::size_t n_resources = 1;
    constexpr std::size_t initial_capacity = 1024;

    ml::MultiArenaMemoryResourcePmr resource{n_resources, initial_capacity};

    EXPECT_THROW(resource.get_resource(1), std::out_of_range);
}
TEST(multi_arena, fill_vectors_with_different_resources) {
    constexpr std::size_t n_resources = 2;
    constexpr std::size_t initial_capacity = 1024;

    ml::MultiArenaMemoryResourcePmr resource{n_resources, initial_capacity};

    // Create vectors using different resources
    auto* arena1 = resource.get_resource(0);
    auto* arena2 = resource.get_resource(1);

    ASSERT_NE(arena1, nullptr);
    ASSERT_NE(arena2, nullptr);

    std::pmr::polymorphic_allocator<int> alloc1{arena1};
    std::pmr::polymorphic_allocator<double> alloc2{arena2};

    std::pmr::vector<int> int_vec{alloc1};
    std::pmr::vector<double> double_vec{alloc2};

    // Fill vectors
    for (int i = 0; i < 10; ++i) {
        int_vec.push_back(i);
        double_vec.push_back(i * 1.1);
    }

    // Validate contents
    EXPECT_EQ(int_vec.size(), 10);
    EXPECT_EQ(double_vec.size(), 10);

    for (std::size_t i{0}; i < 10; ++i) {
        EXPECT_EQ(int_vec[i], i);
        EXPECT_DOUBLE_EQ(double_vec[i], i * 1.1);
    }
}

TEST(multi_t_arena, init) {
    auto resource = ml::MultiTArenaMemoryResourcePmr<int, double, char>{1024};
    SUCCEED();
}
TEST(multi_t_arena, get_resource) {
    auto resource = ml::MultiTArenaMemoryResourcePmr<int, double, char>{1024};
    auto* int_arena = resource.get_resource<int>();
    EXPECT_NE(int_arena, nullptr);
}
