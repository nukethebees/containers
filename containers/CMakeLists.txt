add_library(containers STATIC)
add_library(containers::containers ALIAS containers)

target_sources(containers PRIVATE
  "arena_mmr.cpp"
  "multi_arena_pmr.cpp")
target_sources(containers PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ../
  FILES
  "allocator.hpp"
  "allocator_concepts.hpp"
  "arena_mmr.hpp"
  "arena_mmr_allocator.hpp"
  "arena_mmr_pool.hpp"
  "arena_pmr.hpp"
  "array.hpp"
  "binary_heap.hpp"
  "bst.hpp"
  "bst_iterator.hpp"
  "bst_node.hpp"
  "bubble_sort.hpp"
  "bucket_sort.hpp"
  "buffer_mmr.hpp"
  "contiguous_container_mixins.hpp"
  "buffer_pmr.hpp"
  "dlist.hpp"
  "heap_sort.hpp"
  "insertion_sort.hpp"
  "iterator_boilerplate.hpp"
  "linked_vector.hpp"
  "linked_vector_iterator.hpp"
  "memory_resource_concepts.hpp"
  "merge_sort.hpp"
  "misc.hpp"
  "mmr_allocator.hpp"
  "multi_arena_pmr.hpp"
  "new_delete_pmr.hpp"
  "pmr.hpp"
  "pmr_allocator.hpp"
  "preprocessor/noexcept_release_def.hpp"
  "preprocessor/noexcept_release_undef.hpp"
  "preprocessor/platform_def.hpp"
  "preprocessor/platform_undef.hpp"
  "quick_sort.hpp"
  "radix_sort.hpp"
  "rbset.hpp"
  "resource_mixins.hpp"
  "selection_sort.hpp"
  "slist.hpp"
  "span.hpp"
  "span_iterator.hpp"
  "static_vector.hpp"
  "vector.hpp"
  "vector2.hpp"
)

target_compile_options(containers PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/FAs /Fa${CMAKE_BINARY_DIR}/asm\\>
)

target_compile_definitions(containers PUBLIC
    $<$<CONFIG:Debug>:DEBUG_ENABLED>
)