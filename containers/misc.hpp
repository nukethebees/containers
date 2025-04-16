#pragma once

#include <utility>

namespace ml {
template <typename T>
auto&& max(T&& a, T&& b) {
    return (a < b) ? std::forward<T>(b) : std::forward<T>(a);
}

template <typename... Types>
struct types_to_index {
    template <typename T>
    static constexpr std::size_t get() {
        return get<T, Types...>();
    }

    static constexpr std::size_t size() { return sizeof...(Types); }
  private:
    template <typename T, typename Next, typename... Rest>
    static constexpr std::size_t get() {
        if constexpr (std::is_same_v<T, Next>) {
            return 0;
        } else {
            if constexpr (sizeof...(Rest) == 0) {
                static_assert(!std::is_same_v<T, T>, "Type not found in types_to_index");
            } else {
                return 1 + get<T, Rest...>();
            }
        }
    }
};
}
