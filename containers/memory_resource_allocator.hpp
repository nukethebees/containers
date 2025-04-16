#pragma once

namespace ml {
template <typename T, typename MemoryResource>
class MemoryResourceAllocator {
  public:
    using pointer = T*;
    using const_pointer = T const*;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    MemoryResource* resource_{nullptr};

    template <class U>
    struct rebind {
        using other = MemoryResourceAllocator<U, MemoryResource>;
    };
  public:
    MemoryResourceAllocator() = default;
    MemoryResourceAllocator(MemoryResource* resource)
        : resource_{resource} {}

    MemoryResourceAllocator(MemoryResourceAllocator const&) = default;
    template <typename U>
    MemoryResourceAllocator(MemoryResourceAllocator<U, MemoryResource> const& other)
        : resource_{other.resource_} {}
    MemoryResourceAllocator(MemoryResourceAllocator&&) = default;

    auto operator=(MemoryResourceAllocator const&) -> MemoryResourceAllocator& = default;
    auto operator=(MemoryResourceAllocator&&) -> MemoryResourceAllocator& = default;

    [[nodiscard]] auto allocate(size_type n_elems) -> pointer;
    [[nodiscard]] auto reallocate(pointer p, size_type n_elems) -> pointer;
    auto deallocate(pointer, size_type) -> void;

    template <typename... Args>
    auto construct(pointer p, Args&&... args) -> void;

    template <typename... Args>
    auto construct_new(Args&&... args) -> T&;

    auto operator<=>(MemoryResourceAllocator const&) const noexcept = default;
};

template <typename T, typename MemoryResource>
auto MemoryResourceAllocator<T, MemoryResource>::allocate(size_type n_elems) -> pointer {
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return static_cast<pointer>(resource_->allocate(n_elems * sizeof(T), alignof(T)));
}

template <typename T, typename MemoryResource>
auto MemoryResourceAllocator<T, MemoryResource>::reallocate(pointer p, size_type n_elems) -> pointer {
    if (n_elems == 0) {
        deallocate(p, n_elems);
        return nullptr;
    }
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return static_cast<pointer>(resource_->reallocate(p, n_elems * sizeof(T), alignof(T)));
}

template <typename T, typename MemoryResource>
auto MemoryResourceAllocator<T, MemoryResource>::deallocate(pointer, size_type) -> void {
    // No-op
    // The memory will be deallocated when the resource is destroyed
    return;
}

template <typename T, typename MemoryResource>
template <typename... Args>
auto MemoryResourceAllocator<T, MemoryResource>::construct(pointer p, Args&&... args) -> void {
    if (!p) {
        throw std::bad_alloc{};
    }
    new (p) T(std::forward<Args>(args)...);
}

template <typename T, typename MemoryResource>
template <typename... Args>
auto MemoryResourceAllocator<T, MemoryResource>::construct_new(Args&&... args) -> T& {
    auto* ptr{allocate(1)};
    if (!ptr) {
        throw std::bad_alloc{};
    }

    auto emplaced_value{new (ptr) T(std::forward<Args>(args)...)};
    return *emplaced_value;
}
}
