cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer)

add_library(filter-designer-src
  src/iir/RCGrid.cpp 
  src/iir/IIRFilter.cpp 
  src/SineWave.cpp
  src/fir/FIRFilter.cpp
  src/fir/RectangularWindow.cpp
  src/fir/BlackmanWindow.cpp
  src/FFT.cpp
  src/Sampling.cpp
)

add_executable(filter-designer 
    src/Main.cpp
)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
target_include_directories(filter-designer-src PRIVATE ${FFTW_HEADER_PATH})
target_link_libraries(filter-designer-src ${FFTW_LIB_PATH})

target_link_libraries(filter-designer filter-designer-src)

add_subdirectory(test)