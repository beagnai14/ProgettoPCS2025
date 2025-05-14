cmake_minimum_required(VERSION 3.9)

project(Project LANGUAGES CXX C VERSION 1.0.0)

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
list(APPEND Project_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND Project_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND Project_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND Project_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND Project_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND Project_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(src)

list(APPEND Project_SOURCES ${src_sources})
list(APPEND Project_HEADERS ${src_headers})
list(APPEND Project_INCLUDE ${src_includes})

add_subdirectory(src_test)

list(APPEND Project_TEST_SOURCES ${src_test_sources})
list(APPEND Project_TEST_HEADERS ${src_test_headers})
list(APPEND Project_TEST_INCLUDE ${src_test_includes})

# Create executable
################################################################################
add_executable(${PROJECT_NAME}
      main.cpp
      ${Project_SOURCES}
      ${Project_HEADERS})

target_link_libraries(${PROJECT_NAME} ${Project_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Project_INCLUDE})
target_compile_options(${PROJECT_NAME} PUBLIC -fPIC)

add_executable(${PROJECT_NAME}_TEST
      main_test.cpp
      ${Project_SOURCES}
      ${Project_HEADERS}
      ${src_test_sources}
      ${src_test_headers})

target_link_libraries(${PROJECT_NAME}_TEST ${Project_LINKED_LIBRARIES})
target_include_directories(${PROJECT_NAME}_TEST PRIVATE ${Project_INCLUDE} 
${src_test_includes})
target_compile_options(${PROJECT_NAME}_TEST PUBLIC -fPIC)