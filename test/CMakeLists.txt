set(TESTS_APP_NAME "FilterDesignerTests")

file(GLOB UNIT_TESTS_SRC_FILES 
  ${PROJECT_SOURCE_DIR}/test/*.cpp 
  ${PROJECT_SOURCE_DIR}/test/iir/*.cpp
  ${PROJECT_SOURCE_DIR}/test/fir/*.cpp)

add_executable(${TESTS_APP_NAME}
  ${UNIT_TESTS_SRC_FILES}
)

find_package(Boost 1.85.0 REQUIRED COMPONENTS unit_test_framework)
target_include_directories(${TESTS_APP_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
message("-- Boost Path: " ${Boost_LIBRARY_DIRS} ", Libraries: " ${Boost_LIBRARIES})
target_link_libraries(${TESTS_APP_NAME} FilterDesignerShared Boost::unit_test_framework)
