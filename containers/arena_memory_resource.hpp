#pragma once

#include <cstddef>
#include <memory_resource>

#include "memory_resource_allocator.hpp"

namespace ml {
class ArenaMemoryResource;

class ArenaMemoryResourcePool {
  private:
    ArenaMemoryResourcePool* next_pool_{nullptr};
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
  public:
    ArenaMemoryResourcePool(std::size_t capacity);
    ~ArenaMemoryResourcePool();

    friend class ArenaMemoryResource;

    static auto create_pool(std::size_t initial_size) -> ArenaMemoryResourcePool*;

    auto next_pool() const -> ArenaMemoryResourcePool const*;
    auto total_capacity() const -> std::size_t;
    auto remaining_capacity() const -> std::size_t;
    auto size() const -> std::size_t;
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    void deallocate(void* alloc, std::size_t n_bytes, std::size_t alignment);
};

class ArenaMemoryResource {
  private:
    ArenaMemoryResourcePool* pool_{nullptr};
    ArenaMemoryResourcePool* last_pool_{nullptr};
    std::size_t initial_capacity_{1024};
  public:
    ArenaMemoryResource() = default;
    explicit ArenaMemoryResource(std::size_t initial_capacity);
    ~ArenaMemoryResource();

    ArenaMemoryResource(ArenaMemoryResource const&) = delete;
    ArenaMemoryResource(ArenaMemoryResource&& other);
    auto operator=(ArenaMemoryResource const&) -> ArenaMemoryResource& = delete;
    auto operator=(ArenaMemoryResource&& other) -> ArenaMemoryResource&;

    auto initial_capacity() const -> std::size_t;
    auto pool() const -> ArenaMemoryResourcePool const*;
    auto n_pools() const -> std::size_t;
    auto total_size() const -> std::size_t;
    auto allocate(std::size_t n_bytes, std::size_t alignment) -> void*;
    auto deallocate(void* ptr, std::size_t n_bytes, std::size_t alignment) -> void;
};

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;

class ArenaMemoryResourcePmr : public std::pmr::memory_resource {
  private:
    ArenaMemoryResource arena;
  public:
    ArenaMemoryResourcePmr() = default;
    explicit ArenaMemoryResourcePmr(std::size_t initial_capacity);
    ~ArenaMemoryResourcePmr() override = default;
  protected:
    auto do_allocate(std::size_t n_bytes, std::size_t alignment) -> void* override;
    void do_deallocate(void* p, std::size_t n_bytes, std::size_t alignment) override;
    auto do_is_equal(std::pmr::memory_resource const& other) const noexcept -> bool;
};
}
