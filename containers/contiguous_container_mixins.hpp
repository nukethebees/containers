#pragma once

#include <utility>

namespace ml {
struct ContiguousContainerCommonCapacityMethods {
    template <typename Self>
    constexpr auto empty(this Self const& self) {
        return self.size() == 0;
    }
    template <typename Self>
    constexpr auto size(this Self const& self) {
        return self.size_;
    }
    template <typename Self>
    constexpr auto size_bytes(this Self const& self) {
        return self.size() * sizeof(typename Self::value_type);
    }
};

struct ContiguousContainerCommonMethods {
    // Access
    template <typename Self>
    constexpr auto&& at(this Self&& self, auto i) {
        if (i >= self.size()) {
            throw std::out_of_range{"Index out of range"};
        }
        return std::forward<Self>(self).data()[i];
    }
    template <typename Self>
    constexpr auto* data(this Self&& self) {
        return std::forward<Self>(self).data_;
    }
    template <typename Self>
    constexpr auto&& back(this Self&& self) {
        return std::forward<Self>(self).data()[self.size() - 1];
    }

    template <typename Self>
    constexpr auto&& front(this Self&& self) {
        return std::forward<Self>(self).data()[0];
    }

    template <typename Self>
    constexpr auto&& operator[](this Self&& self, auto i) {
        return std::forward<Self>(self).data()[i];
    }
};
}
