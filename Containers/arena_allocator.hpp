#pragma once

#include <compare>
#include <cstddef>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <vector>

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
class ArenaAllocator {
public:
    using pointer = T *;
    using const_pointer = T const *;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    // private:
    ArenaMemoryResource * resource_{nullptr};
public:
    ArenaAllocator() = default;
    ArenaAllocator(ArenaMemoryResource * resource)
        : resource_{resource} {}

    ArenaAllocator(ArenaAllocator const &) = default;
    template <typename U>
    ArenaAllocator(ArenaAllocator<U> const & other)
        : resource_{other.resource_} {}
    ArenaAllocator(ArenaAllocator &&) = default;

    auto operator=(ArenaAllocator const &)->ArenaAllocator & = default;
    auto operator=(ArenaAllocator &&)->ArenaAllocator & = default;

    [[nodiscard]] auto allocate(size_type n_elems) -> pointer {
        if (!resource_) {
            throw std::bad_alloc{};
        }
        return static_cast<pointer>(resource_->allocate(n_elems * sizeof(T), alignof(T)));
    }
    [[nodiscard]] auto reallocate(pointer p, size_type n_elems) -> pointer {
        if (n_elems == 0) {
            deallocate(p, n_elems);
            return nullptr;
        }
        if (!resource_) {
            throw std::bad_alloc{};
        }
        return static_cast<pointer>(resource_->reallocate(p, n_elems * sizeof(T), alignof(T)));
    }
    auto deallocate(pointer, size_type) -> void {
        // No-op
        // The memory will be deallocated when the resource is destroyed
        return;
    }
    template <typename... Args>
    auto construct(pointer p, Args&&... args) -> void {
        if (!p) {
            throw std::bad_alloc{};
        }
        new (p) T(std::forward<Args>(args)...);
    }
    template <typename... Args>
    auto construct_new(Args&&... args) -> T & {
        auto * ptr{allocate(1)};
        if (!ptr) {
            throw std::bad_alloc{};
        }

        auto emplaced_value{new (ptr) T(std::forward<Args>(args)...)};
        return *emplaced_value;
    }

    auto operator<=>(ArenaAllocator const &) const noexcept = default;
};
}
