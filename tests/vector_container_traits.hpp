#pragma once

template <typename Container>
struct vector_container_traits {
    template <typename U>
    static void add_element(Container& container, U&& value) {
        container.push_back(std::forward<U>(value));
    }
    template <typename Elem, typename... Remaining>
    static auto add_elements(Container& container, Elem&& elem, Remaining&&... remaining) -> decltype(auto) {
        add_element(container, std::forward<Elem>(elem));
        if constexpr (sizeof...(remaining)) {
            add_elements(container, std::forward<Remaining>(remaining)...);
        }
    }

    template <typename... Args>
    static auto emplace_element(Container& container, Args&&... args) -> decltype(auto) {
        return container.emplace_back(std::forward<Args>(args)...);
    }
};
