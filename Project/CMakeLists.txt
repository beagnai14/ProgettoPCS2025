cmake_minimum_required(VERSION 3.9)

project(PolygonalMesh LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND PolygonalMesh_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND PolygonalMesh_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND PolygonalMesh_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND PolygonalMesh_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND PolygonalMesh_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND PolygonalMesh_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(src)

list(APPEND PolygonalMesh_SOURCES ${polygonalMesh_sources})
list(APPEND PolygonalMesh_HEADERS ${polygonalMesh_headers})
list(APPEND PolygonalMesh_INCLUDE ${polygonalMesh_includes})

add_subdirectory(ExportParaview)

list(APPEND PolygonalMesh_SOURCES ${src_paraview_sources})
list(APPEND PolygonalMesh_HEADERS ${src_paraview_headers})
list(APPEND PolygonalMesh_INCLUDE ${src_paraview_includes})

# Create executable
################################################################################
add_executable(${PROJECT_NAME}
      main.cpp
      ${PolygonalMesh_SOURCES}
      ${PolygonalMesh_HEADERS})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell0Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell1Ds.csv  ${CMAKE_CURRENT_SOURCE_DIR}/PolygonalMesh/Cell2Ds.csv
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${PolygonalMesh_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PolygonalMesh_INCLUDE})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)
