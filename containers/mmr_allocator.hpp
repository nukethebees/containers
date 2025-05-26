#pragma once

namespace ml {
template <typename T, typename MemoryResource>
class mmr_allocator {
  public:
    using pointer = T*;
    using const_pointer = T const*;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = mmr_allocator<U, MemoryResource>;
    };

    mmr_allocator() = default;
    mmr_allocator(MemoryResource* resource)
        : resource_{resource} {}

    mmr_allocator(mmr_allocator const&) = default;
    template <typename U>
    mmr_allocator(mmr_allocator<U, MemoryResource> const& other)
        : resource_{const_cast<MemoryResource*>(other.resource())} {}
    mmr_allocator(mmr_allocator&&) = default;

    auto operator=(mmr_allocator const&) -> mmr_allocator& = default;
    auto operator=(mmr_allocator&&) -> mmr_allocator& = default;

    // Access
    auto resource() -> MemoryResource* { return resource_; }
    auto resource() const -> MemoryResource const* { return resource_; }

    // Allocation
    [[nodiscard]] auto allocate(size_type n_elems) -> pointer;
    [[nodiscard]] auto allocate_bytes(size_type n_bytes, size_type alignment) -> void*;
    auto deallocate(pointer ptr, size_type n_elems) -> void;
    auto deallocate_bytes(void* ptr, size_type n_bytes, size_type alignment) -> void;
    [[nodiscard]] auto extend_bytes(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void*;

    // Object construction
    template <typename... Args>
    auto construct(pointer p, Args&&... args) -> void;
    template <typename... Args>
    auto construct_new(Args&&... args) -> T&;

    auto operator<=>(mmr_allocator const&) const noexcept = default;
  private:
    MemoryResource* resource_{nullptr};
};

template <typename T, typename MemoryResource>
inline auto mmr_allocator<T, MemoryResource>::allocate(size_type n_elems) -> pointer {
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return static_cast<pointer>(resource_->allocate(n_elems * sizeof(T), alignof(T)));
}
template <typename T, typename MemoryResource>
inline auto mmr_allocator<T, MemoryResource>::allocate_bytes(size_type n_bytes, size_type alignment) -> void* {
    if (!resource_) {
        throw std::bad_alloc{};
    }
    return resource_->allocate(n_bytes, alignment);
}
template <typename T, typename MemoryResource>
inline auto mmr_allocator<T, MemoryResource>::extend_bytes(void* ptr,
                                                           size_type old_bytes,
                                                           size_type new_bytes,
                                                           size_type alignment) -> void* {
    return resource_->extend(ptr, old_bytes, new_bytes, alignment);
}

template <typename T, typename MemoryResource>
inline auto mmr_allocator<T, MemoryResource>::deallocate(pointer ptr, size_type n_elems) -> void {
    auto const n_bytes{n_elems * sizeof(T)};
    return deallocate_bytes(ptr, n_bytes, alignof(T));
}
template <typename T, typename MemoryResource>
inline auto mmr_allocator<T, MemoryResource>::deallocate_bytes(void* ptr, size_type n_bytes, size_type alignment)
    -> void {
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
inline auto mmr_allocator<T, MemoryResource>::construct(pointer p, Args&&... args) -> void {
    if (!p) {
        throw std::bad_alloc{};
    }
    new (p) T(std::forward<Args>(args)...);
}

template <typename T, typename MemoryResource>
template <typename... Args>
inline auto mmr_allocator<T, MemoryResource>::construct_new(Args&&... args) -> T& {
    auto* ptr{allocate(1)};
    if (!ptr) {
        throw std::bad_alloc{};
    }

    auto emplaced_value{new (ptr) T(std::forward<Args>(args)...)};
    return *emplaced_value;
}
}
