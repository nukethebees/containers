# Containers

My implementations of containers, sorting algorithms, and allocators.

Unit tests are found in `tests` and benchmarks in `benchmarks`.

C++23 is required to build the code.

# Containers

| Name | Description | STL equivalent |
|------|-------------| ----------------|
| `array` | Fixed-size array | `std::array` |
| `vector` | Dynamic array | `std::vector` |
| `slist` | Singly-linked list | `std::forward_list` |
| `dlist` | Doubly-linked list | `std::list` |
| `bst` | Binary search tree | N/A |
| `linked_vector` | Singly-linked list / vector hybrid | N/A |
| `binary_heap` | A binary heap | `std::vector` with `std::make_heap` |

# Sorting Algorithms

The sorting algorithms use `STL`-style iterators.

- Bubble sort
- Bucket sort
- Counting sort
- Heap sort
- Insertion sort
- Merge sort
- Quick sort
- Selection sort

# Allocators

`ml::allocator` extends `std::allocator` with the `allocate_bytes` and `deallocate_bytes` member functions
found in `std::pmr::polymorphic_allocator`.

- `pmr` means "polymorphic memory resource".
- `mmr` means "monomorphic memory resource".

I have implemented a version of `std::pmr::polymorphic_allocator` but with the ability to extend allocations.
The polymorphic memory resources inherit from `ml::memory_resource` and thus can be used with the polymorphic allocator
at runtime. In contrast, the monomorphic memory resources require `ml::mmr_allocator` to be specialised for
each separate memory resource, creating a new type.

The allocation extension capability of `ml::polymorphic_allocator` enables a vector implementation which 
can ask the allocator to try and extend the current allocation instead of always reallocating and 
moving/coping the elements.

| Name | Description |
|------|-------------|
| `allocator` | Inherits `std::allocator` but adds `(de)allocate_bytes` from `std::pmr::polymorphic_allocator` |
| `arena_mmr` | Monomorphic pooled arena resource |
| `arena_pmr` | Polymorphic pooled arena resource |
| `buffer_mmr` | Monomorphic stack-allocated buffer resource |
| `buffer_pmr` | Polymorphic stack-allocated buffer resource |
| `mmr_allocator` | Allocator for `mmr` memory resources |
| `pmr_allocator` | Allocator for `pmr` memory resources |

# Other classes

| Name | Description | STL equivalent |
|------|-------------| ----------------|
| `span` | Contiguous memory view | `std::span` |

`ml::span` works with incomplete types unlike `std::span`.

# Code style and naming conventions

Types use snake case as in the standard library, e.g. `my_list`.
All code is within the `ml` namespace.