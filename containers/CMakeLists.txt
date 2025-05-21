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
  "bst.hpp"
  "bst_iterator.hpp"
  "bst_node.hpp"
  "bubble_sort.hpp"
  "buffer_mmr.hpp"
  "buffer_pmr.hpp"
  "dlist.hpp"
  "insertion_sort.hpp"
  "linked_vector.hpp"
  "linked_vector_iterator.hpp"
  "memory_resource_concepts.hpp"
  "misc.hpp"
  "mmr_allocator.hpp"
  "multi_arena_pmr.hpp"
  "pmr.hpp"
  "pmr_allocator.hpp"
  "rbset.hpp"
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