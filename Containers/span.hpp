#pragma once

#include <cstddef>

namespace ml {
template <typename T>
class span {
public:
    using value_t = T;
    using ptr_t = T *;
    using size_t = std::size_t;
private:
    ptr_t ptr;
    size_t size;
private:
    span() = delete;
    span(ptr_t ptr_, size_t size_)
        : ptr{ptr_}
        , size{size_} {}
};
}