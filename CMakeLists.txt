cmake_minimum_required(VERSION 3.10)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer VERSION 0.1 LANGUAGES CXX)

add_subdirectory(shared)
add_subdirectory(gui)
add_subdirectory(test)