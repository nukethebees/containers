add_library(containers STATIC)
add_library(containers::containers ALIAS containers)

target_sources(containers PRIVATE
  "arena_allocator.cpp"
 )

target_sources(containers PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ../
  FILES
  "arena_allocator.hpp"
  "misc.hpp"
  "span.hpp"
  "span_iterator.hpp"
  "stack_buffer_memory_resource.hpp"
  "vector.hpp"
)