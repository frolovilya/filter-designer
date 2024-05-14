cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer)

set(FFTW_LIB fftw3)
set(FFTW_HEADER fftw3.h)
find_path(FFTW_HEADER_PATH ${FFTW_HEADER})
find_library(FFTW_LIB_PATH ${FFTW_LIB})

add_executable(${CMAKE_PROJECT_NAME} 
    src/main.cpp 
    src/RCGrid.cpp 
    src/IIRFilter.cpp 
    src/SineWave.cpp
    src/FIRFilter.cpp
    src/RectangularWindow.cpp
    src/BlackmanWindow.cpp
    src/FFT.cpp
    src/Sampling.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${FFTW_HEADER_PATH})

target_link_libraries(${CMAKE_PROJECT_NAME} ${FFTW_LIB_PATH})