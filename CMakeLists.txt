cmake_minimum_required(VERSION 3.10)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
message("Build type: " ${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer VERSION 1.0.3 LANGUAGES CXX)

message("-- App Version: " ${PROJECT_VERSION_MAJOR} "." ${PROJECT_VERSION_MINOR} " (build " ${PROJECT_VERSION_PATCH} ")")

add_subdirectory(shared)
add_subdirectory(gui)
add_subdirectory(test)