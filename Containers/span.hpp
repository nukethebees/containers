#pragma once

#include <cstddef>
#include <type_traits>
#include <concepts>

namespace ml {
template <typename T>
concept has_data_and_size = requires (T t) {

    { t.size() } -> std::same_as<std::size_t>;
    requires std::is_pointer_v<decltype(t.data())>;
};

template <typename T>
class span {
public:
    using value_t = T;
    using ptr_t = T *;
    using const_ptr_t = T const *;
    using size_t = std::size_t;
private:
    ptr_t data_;
    size_t size_;
public:
    span() = delete;
    span(ptr_t ptr, size_t size) noexcept
        : data_{ptr}
        , size_{size} {}
    template <typename U>
        requires has_data_and_size<U>
    span(U & container) noexcept
        : data_{container.data()}
        , size_{container.size()} {}

    auto size() const -> size_t {
        return size_;
    }
    auto data() const -> const_ptr_t {
        return data_;
    }
    auto data() -> ptr_t {
        return data_;
    }
};

template <typename T>
    requires has_data_and_size<T>
span(T& iter) -> span<std::remove_cvref_t<decltype(*iter.data())>>;
}