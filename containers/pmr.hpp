#pragma once

#include <cstddef>

namespace ml {
class pmr {
  public:
    using size_type = std::size_t;

    pmr() = default;
    virtual ~pmr() noexcept = default;

    // Allocation
    auto allocate(size_type n_bytes, size_type alignment) -> void* { return do_allocate(n_bytes, alignment); }
    void deallocate(void* ptr, size_type n_bytes, size_type alignment) { do_deallocate(ptr, n_bytes, alignment); }
    auto extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* {
        return do_extend(ptr, old_bytes, new_bytes, alignment);
    }
    // Comparison
    auto is_equal(pmr const& other) const noexcept -> bool { return do_is_equal(other); }
  private:
    // Allocation
    virtual auto do_allocate(size_type n_bytes, size_type alignment) -> void* = 0;
    virtual void do_deallocate(void* ptr, size_type n_bytes, size_type alignment) = 0;
    virtual auto do_extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment) -> void* = 0;
    // Comparison
    virtual auto do_is_equal(pmr const& other) const noexcept -> bool = 0;
};
}
