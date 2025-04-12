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

class ArenaMemoryResource2;

class Pool2 {
    Pool2 * next_pool_{nullptr};
    std::byte * buffer{nullptr}; // Non-owning pointer to the buffer
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
public:
    Pool2(std::byte * buffer, std::size_t capacity);
    ~Pool2();

    static auto create_pool(std::size_t initial_size) -> Pool2 *;

    auto next_pool() const->Pool2 const *;
    auto total_capacity() const->std::size_t;
    auto remaining_capacity() const->std::size_t;
    auto size() const->std::size_t;
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment, ArenaMemoryResource2 & resource) -> void *; // Updated
    void deallocate(void * alloc, std::size_t n_bytes, std::size_t alignment);
};

class ArenaMemoryResource2 {
    //private:
public:
    Pool2 * pool_{nullptr};
    Pool2 * last_pool_{nullptr}; // Added member to track the last pool
    std::size_t initial_capacity_{1024};
public:
    ArenaMemoryResource2() = default;
    explicit ArenaMemoryResource2(std::size_t initial_capacity);
    ~ArenaMemoryResource2();

    ArenaMemoryResource2(ArenaMemoryResource2 const &) = delete;
    ArenaMemoryResource2(ArenaMemoryResource2 && other);
    auto operator=(ArenaMemoryResource2 const &) -> ArenaMemoryResource2 & = delete;
    auto operator=(ArenaMemoryResource2 && other) -> ArenaMemoryResource2 &;

    auto initial_capacity() const->std::size_t;
    auto pool() const->Pool2 const *;
    auto n_pools() const->std::size_t;
    auto total_size() const->std::size_t;
    auto allocate(std::size_t n_bytes, std::size_t alignment) -> void *;
    auto deallocate(void * ptr, std::size_t n_bytes, std::size_t alignment) -> void;
private:
    void create_pool();
};

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;
template <typename T>
using ArenaAllocator2 = MemoryResourceAllocator<T, ArenaMemoryResource2>;

}