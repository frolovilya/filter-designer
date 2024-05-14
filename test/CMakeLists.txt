cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB UNIT_TESTS_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(filter-designer-tests
  ${UNIT_TESTS_SRC_FILES}
)

find_package(Boost 1.85.0 REQUIRED COMPONENTS unit_test_framework)
target_include_directories(filter-designer-tests PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(filter-designer-tests filter-designer-src Boost::unit_test_framework)