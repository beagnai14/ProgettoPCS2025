list(APPEND src_test_headers ${CMAKE_CURRENT_SOURCE_DIR}/test_polyhedron.hpp)

list(APPEND src_test_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(src_test_sources ${src_test_sources} PARENT_SCOPE)
set(src_test_headers ${src_test_headers} PARENT_SCOPE)
set(src_test_includes ${src_test_includes} PARENT_SCOPE)