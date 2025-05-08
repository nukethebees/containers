#pragma once

namespace ml {
template <typename T>
class linked_vector_segment {
  private:
    linked_vector_segment* prev_{nullptr};
    linked_vector_segment* next_{nullptr};
    T* data_{nullptr};
};
}
