#pragma once

#include <cstddef>
#include <compare>
#include <memory_resource>

namespace ml {
// A copy of the std polymorphic allocator from C++20
// The main difference is that this one propagates the memory resource when the container is copied
template <typename T = std::byte>
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
  private:
    std::pmr::memory_resource* resource_{nullptr};

    template <class U>
    friend class polymorphic_allocator;
  public:
    // Constructors
    polymorphic_allocator() = delete;
    polymorphic_allocator(std::pmr::memory_resource* resource)
        : resource_{resource} {}
    // Copy/move constructors
    polymorphic_allocator(polymorphic_allocator const&) noexcept = default;
    template <typename U>
    polymorphic_allocator(polymorphic_allocator<U> const& other) noexcept
        : resource_{other.resource_} {}
    polymorphic_allocator(polymorphic_allocator&&) noexcept = default;
    // Assignment operators
    auto operator=(polymorphic_allocator const&) noexcept -> polymorphic_allocator& = default;
    auto operator=(polymorphic_allocator&&) noexcept -> polymorphic_allocator& = default;

    // Allocation
    auto allocate(size_type n) -> T* { return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T))); }
    auto allocate_bytes(size_type n, size_type alignment) -> void* { return resource_->allocate_bytes(n, alignment); }
    // Deallocation
    void deallocate(T* p, size_type n) noexcept {
        if (p) {
            resource_->deallocate(p, n * sizeof(T), alignof(T));
        }
    }
    void deallocate_bytes(void* p, size_type n, size_type alignment) { resource_->deallocate_bytes(p, n, alignment); }

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
    auto resource() const noexcept -> std::pmr::memory_resource* { return resource_; }
    auto max_size() const noexcept -> size_type { return std::numeric_limits<size_type>::max() / sizeof(T); }

    // Comparison
    auto operator<=>(polymorphic_allocator const&) const noexcept = default;
};
}
