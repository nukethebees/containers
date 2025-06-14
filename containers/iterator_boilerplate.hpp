#pragma once

namespace ml {
struct IteratorReverseMethods {
    template <typename Self>
    auto crbegin(this Self const& self) {
        using T = std::remove_cvref_t<Self>;
        using const_reverse_iterator = typename T::const_reverse_iterator;
        return const_reverse_iterator(self.end());
    }
    template <typename Self>
    auto crend(this Self const& self) {
        using T = std::remove_cvref_t<Self>;
        using const_reverse_iterator = typename T::const_reverse_iterator;
        return const_reverse_iterator(self.begin());
    }
    template <typename Self>
    auto rbegin(this Self&& self) {
        using T = std::remove_cvref_t<Self>;
        using reverse_iterator = typename T::reverse_iterator;
        using const_reverse_iterator = typename T::const_reverse_iterator;

        if constexpr (std::is_const_v<std::remove_reference_t<Self>>) {
            return const_reverse_iterator(std::forward<Self>(self).end());
        } else {
            return reverse_iterator(std::forward<Self>(self).end());
        }
    }
    template <typename Self>
    auto rend(this Self&& self) {
        using T = std::remove_cvref_t<Self>;
        using reverse_iterator = typename T::reverse_iterator;
        using const_reverse_iterator = typename T::const_reverse_iterator;

        if constexpr (std::is_const_v<std::remove_reference_t<Self>>) {
            return const_reverse_iterator(std::forward<Self>(self).begin());
        } else {
            return reverse_iterator(std::forward<Self>(self).begin());
        }
    }
};

struct ContiguousIteratorMethods : public IteratorReverseMethods {
    template <typename Self>
    auto begin(this Self&& self) {
        using T = std::remove_cvref_t<Self>;
        using iterator = typename T::iterator;
        using const_iterator = typename T::const_iterator;

        if constexpr (std::is_const_v<std::remove_reference_t<Self>>) {
            return const_iterator(std::forward<Self>(self).data());
        } else {
            return iterator(std::forward<Self>(self).data());
        }
    }
    template <typename Self>
    auto cbegin(this Self const& self) {
        using T = std::remove_cvref_t<Self>;
        using const_iterator = typename T::const_iterator;
        return const_iterator(self.data());
    }
    template <typename Self>
    auto cend(this Self const& self) {
        using T = std::remove_cvref_t<Self>;
        using const_iterator = typename T::const_iterator;
        return const_iterator(self.data() + self.size());
    }
    template <typename Self>
    auto end(this Self&& self) {
        using T = std::remove_cvref_t<Self>;
        using iterator = typename T::iterator;
        using const_iterator = typename T::const_iterator;

        if constexpr (std::is_const_v<std::remove_reference_t<Self>>) {
            return const_iterator(self.data() + std::forward<Self>(self).size());
        } else {
            return iterator(self.data() + std::forward<Self>(self).size());
        }
    }
};
}
