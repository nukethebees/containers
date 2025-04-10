#pragma once

namespace ml {
template <typename T>
auto&& max(T&& a, T&& b) {
    return (a < b) ? std::forward<T>(b) : std::forward<T>(a);
}
}
