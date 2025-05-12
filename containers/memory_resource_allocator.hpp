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
    [[nodiscard]] auto allocate_bytes(size_type n_bytes, size_type alignment) -> pointer;
    auto deallocate(pointer ptr, size_type n_elems) -> void;
    auto deallocate_bytes(pointer ptr, size_type n_bytes, size_type alignment) -> void;

    template <typename... Args>
    auto construct(pointer p, Args&&... args) -> void;

    template <typename... Args>
    auto construct_new(Args&&... args) -> T&;

    auto operator<=>(MemoryResourceAllocator const&) const noexcept = default;
};

template <typename T, typename MemoryResource>
inline auto MemoryResourceAllocator<T, MemoryResource>::allocate(size_type n_elems) -> pointer {
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return static_cast<pointer>(resource_->allocate(n_elems * sizeof(T), alignof(T)));
}
template <typename T, typename MemoryResource>
inline auto MemoryResourceAllocator<T, MemoryResource>::allocate_bytes(size_type n_bytes, size_type alignment)
    -> pointer {
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return static_cast<pointer>(resource_->allocate(n_bytes, alignment));
}

template <typename T, typename MemoryResource>
inline auto MemoryResourceAllocator<T, MemoryResource>::deallocate(pointer ptr, size_type n_elems) -> void {
    auto const n_bytes{n_elems * sizeof(T)};
    return deallocate_bytes(ptr, n_bytes, alignof(T));
}
template <typename T, typename MemoryResource>
inline auto MemoryResourceAllocator<T, MemoryResource>::deallocate_bytes(pointer ptr,
                                                                         size_type n_bytes,
                                                                         size_type alignment) -> void {
    if (!ptr) {
        return;
    }
    if (!resource_) {
        throw std::bad_alloc{};
    }
    resource_->deallocate(ptr, n_bytes, alignment);
    return;
}

template <typename T, typename MemoryResource>
template <typename... Args>
inline auto MemoryResourceAllocator<T, MemoryResource>::construct(pointer p, Args&&... args) -> void {
    if (!p) {
        throw std::bad_alloc{};
    }
    new (p) T(std::forward<Args>(args)...);
}

template <typename T, typename MemoryResource>
template <typename... Args>
inline auto MemoryResourceAllocator<T, MemoryResource>::construct_new(Args&&... args) -> T& {
    auto* ptr{allocate(1)};
    if (!ptr) {
        throw std::bad_alloc{};
    }

    auto emplaced_value{new (ptr) T(std::forward<Args>(args)...)};
    return *emplaced_value;
}
}
