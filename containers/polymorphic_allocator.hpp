#pragma once

#include <compare>
#include <memory_resource>

namespace ml {
// A copy of the std polymorphic allocator from C++20
// The main difference is that this one propagates the memory resource when the container is copied
template <typename T>
class polymorphic_allocator {
  public:
    using pointer = T*;
    using const_pointer = T const*;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
  private:
    std::pmr::memory_resource* resource_{nullptr};

    template <class U>
    friend class polymorphic_allocator;
  public:
    polymorphic_allocator() = delete;
    polymorphic_allocator(std::pmr::memory_resource* resource)
        : resource_{resource} {}

    polymorphic_allocator(polymorphic_allocator const&) noexcept = default;
    template <typename U>
    polymorphic_allocator(polymorphic_allocator<U> const& other) noexcept
        : resource_{other.resource_} {}
    polymorphic_allocator(polymorphic_allocator&&) noexcept = default;

    auto allocate(std::size_t n) -> T* { return static_cast<T*>(resource_->allocate(n * sizeof(T), alignof(T))); }
    void deallocate(T* p, std::size_t n) noexcept {
        if (p) {
            resource_->deallocate(p, n * sizeof(T), alignof(T));
        }
    }

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

    auto select_on_container_copy_construction() const noexcept -> polymorphic_allocator { return *this; }

    auto resource() const noexcept -> std::pmr::memory_resource* { return resource_; }

    auto operator=(polymorphic_allocator const&) noexcept -> polymorphic_allocator& = default;
    auto operator=(polymorphic_allocator&&) noexcept -> polymorphic_allocator& = default;

    auto operator<=>(polymorphic_allocator const&) const noexcept = default;
};
}
