#pragma once

#include <compare>
#include <cstddef>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <vector>

#include "memory_resource_allocator.hpp"

namespace ml {
class ArenaMemoryResource {
public:
    class Pool {
    private:
        std::unique_ptr<std::byte[]> buffer{nullptr};
        std::size_t total_capacity_{0};
        std::size_t remaining_capacity_{0};
        void * last_allocation_{nullptr};
    public:
        Pool() = delete;
        explicit Pool(std::size_t capacity);

        auto total_capacity() const->std::size_t;
        auto remaining_capacity() const->std::size_t;
        auto size() const->std::size_t;

        [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void *;
        [[nodiscard]] auto reallocate(void * alloc, std::size_t new_size_bytes, std::size_t alignment) -> void *;
    };
private:
    std::vector<Pool> pools_;
    std::size_t initial_capacity_{1024};
public:
    static constexpr std::size_t growth_factor{2};

    ArenaMemoryResource() noexcept = default;
    ArenaMemoryResource(std::size_t initial_capacity) noexcept;

    auto get_sufficient_pool(std::size_t n_bytes) -> Pool &;
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void *;
    [[nodiscard]] auto reallocate(void * alloc, std::size_t n_bytes, std::size_t alignment) -> void *;
    auto initial_capacity() const->std::size_t;

    template <typename Self>
    auto pools(this Self && self) {
        return std::span<Pool const>(std::forward<Self>(self).pools_);
    }
};

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;
}
