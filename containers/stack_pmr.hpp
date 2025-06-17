#pragma once

namespace ml {
template <typename resource_base>
class stack_pmr : public resource_base {
    auto do_allocate(size_type n_bytes, size_type alignment) -> void* override final {
        return nullptr;
    }
    void do_deallocate(void* ptr, size_type n_bytes, size_type alignment) override final { return; }
    virtual auto do_extend(void* ptr, size_type old_bytes, size_type new_bytes, size_type alignment)
        -> void* final {
        return nullptr;
    }
    auto do_is_equal(resource_base const& other) const noexcept -> bool override final {
        return this == &other;
    }
};
}
