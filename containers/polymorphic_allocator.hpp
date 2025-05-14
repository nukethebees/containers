#pragma once

#include <cstddef>
#include <compare>
#include <memory_resource>

#include "allocator_concepts.hpp"
#include "memory_resource.hpp"

namespace ml {
// A copy of the std polymorphic allocator from C++20
// The main difference is that this one propagates the memory resource when the container is copied
template <typename T = std::byte, typename ResourceT = ml::memory_resource>
class polymorphic_allocator {
  public:
    using pointer = T*;
    using const_pointer = T const*;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

    template <typename NewAllocT>
    struct rebind {
        using other = polymorphic_allocator<NewAllocT, ResourceT>;
    };

    // Constructors
    polymorphic_allocator() = delete;
    polymorphic_allocator(ResourceT* resource)
        : resource_{resource} {}
    // Copy/move constructors
    polymorphic_allocator(polymorphic_allocator const&) noexcept = default;
    template <typename U>
    polymorphic_allocator(polymorphic_allocator<U, ResourceT> const& other) noexcept
        : resource_{other.resource()} {}
    polymorphic_allocator(polymorphic_allocator&&) noexcept = default;
    // Assignment operators
    auto operator=(polymorphic_allocator const&) noexcept -> polymorphic_allocator& = default;
    auto operator=(polymorphic_allocator&&) noexcept -> polymorphic_allocator& = default;

    // Allocation
    auto allocate(size_type n) -> T* { return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T))); }
    auto allocate_bytes(size_type n, size_type alignment) -> void* { return resource_->allocate(n, alignment); }
    // Deallocation
    void deallocate(T* p, size_type n) noexcept {
        if (p) {
            resource_->deallocate(p, n * sizeof(T), alignof(T));
        }
    }
    void deallocate_bytes(void* p, size_type n, size_type alignment) { resource_->deallocate(p, n, alignment); }
    // Extension
    auto extend(T* ptr, size_type old_elems, size_type new_elems) -> T* {
        if constexpr (extendable_memory_resource<ResourceT>) {
            return static_cast<T*>(resource_->extend(ptr, old_elems * sizeof(T), new_elems * sizeof(T), alignof(T)));
        } else {
            return resource_->allocate(new_elems * sizeof(T), alignof(T));
        }
    }
    auto extend_bytes(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* {
        if constexpr (extendable_memory_resource<ResourceT>) {
            return resource_->extend(ptr, old_bytes, new_bytes, alignment);
        } else {
            return resource_->allocate(new_bytes, alignment);
        }
    }

    // Object construction/destruction
    template <class U, class... Args>
    void construct(U* p, Args&&... args) {
        if (!p) {
            throw std::bad_alloc{};
        }
        new (p) U(std::forward<Args>(args)...);
    }
    template <class U>
    void destroy(U* p) {
        if (p) {
            p->~U();
        }
    }

    // Misc
    auto select_on_container_copy_construction() const noexcept -> polymorphic_allocator { return *this; }
    auto resource() const noexcept -> ResourceT* { return resource_; }
    auto max_size() const noexcept -> size_type { return std::numeric_limits<size_type>::max() / sizeof(T); }

    // Comparison
    auto operator<=>(polymorphic_allocator const&) const noexcept = default;
  private:
    ResourceT* resource_{nullptr};
};
}
