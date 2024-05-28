file(GLOB UNIT_TESTS_SRC_FILES 
  ${PROJECT_SOURCE_DIR}/test/*.cpp 
  ${PROJECT_SOURCE_DIR}/test/iir/*.cpp
  ${PROJECT_SOURCE_DIR}/test/fir/*.cpp)

add_executable(filter-designer-tests
  ${UNIT_TESTS_SRC_FILES}
)

find_package(Boost 1.85.0 REQUIRED COMPONENTS unit_test_framework)
target_include_directories(filter-designer-tests PRIVATE ${Boost_INCLUDE_DIRS})
message("-- Boost Path: " ${Boost_LIBRARY_DIRS} ", Libraries: " ${Boost_LIBRARIES})
target_link_libraries(filter-designer-tests filter-designer-shared Boost::unit_test_framework)
