#pragma once

#include "pmr.hpp"

namespace ml {
template <typename resource_base>
class new_delete_pmr : public resource_base {
  public:
    using size_type = std::size_t;

    new_delete_pmr() = default;
    ~new_delete_pmr() override = default;

    auto do_allocate(size_type n_bytes, size_type alignment) -> void* override final {
        return ::operator new(n_bytes, std::align_val_t{alignment});
    }
    void do_deallocate(void* ptr, size_type n_bytes, size_type alignment) override final {
        ::operator delete(ptr, n_bytes, std::align_val_t{alignment});
    }
    virtual auto
        do_extend(void* /*ptr*/, size_type /*old_bytes*/, size_type new_bytes, size_type alignment)
            -> void* final {
        return do_allocate(new_bytes, alignment);
    }
    auto do_is_equal(resource_base const& other) const noexcept -> bool override final {
        return this == &other;
    }
};

template <typename resource_base = ml::pmr>
inline static auto* get_new_delete_pmr() {
    static auto resource{ml::new_delete_pmr<resource_base>()};
    return &resource;
}
}
