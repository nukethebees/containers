#pragma once

// Used in type-parameterised tests
// Contains a lambda which constructs the container for the test
template <typename ContainerT, auto ctor>
struct ContainerDefinition {
    using Container = ContainerT;
    static constexpr auto& construct = ctor;
};
