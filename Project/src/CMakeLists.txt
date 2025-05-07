list(APPEND polygonalMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/Utils.hpp)
list(APPEND polygonalMesh_sources ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp)

list(APPEND polygonalMesh_headers ${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh.hpp)

list(APPEND polygonalMesh_includes ${CMAKE_CURRENT_SOURCE_DIR})

set(polygonalMesh_sources ${polygonalMesh_sources} PARENT_SCOPE)
set(polygonalMesh_headers ${polygonalMesh_headers} PARENT_SCOPE)
set(polygonalMesh_includes ${polygonalMesh_includes} PARENT_SCOPE)
