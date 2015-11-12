INCLUDE_DIRECTORIES(
  include/
  ${CMAKE_CURRENT_BINARY_DIR}
)

SET(motr_SRC
  src/core/version.cpp
  src/core/allocator.cpp
  src/stack_memory_pool.cpp
)

ADD_LIBRARY(motrlib SHARED ${motr_SRC})