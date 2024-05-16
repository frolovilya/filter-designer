cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer VERSION 0.1 LANGUAGES CXX)

add_library(filter-designer-src
  src/iir/RCGrid.cpp src/iir/RCGrid.hpp
  src/iir/IIRFilter.cpp src/iir/IIRFilter.hpp
  src/SineWave.cpp src/SineWave.hpp
  src/fir/FIRFilter.cpp src/fir/FIRFilter.hpp
  src/fir/Window.cpp src/fir/Window.hpp
  src/fir/RectangularWindow.cpp src/fir/RectangularWindow.hpp
  src/fir/BlackmanWindow.cpp src/fir/BlackmanWindow.hpp
  src/FFT.cpp src/FFT.hpp
  src/Sampling.cpp src/Sampling.hpp
)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
target_include_directories(filter-designer-src PRIVATE ${FFTW_HEADER_PATH})
target_link_libraries(filter-designer-src ${FFTW_LIB_PATH})


# QT App

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "~/Dev/tools/QT/6.7.0/macos")
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Charts)

qt_standard_project_setup()

qt_add_executable(filter-designer
    src/main.cpp
)

qt_add_qml_module(filter-designer
    URI QT
    VERSION 1.0
    NO_RESOURCE_TARGET_PATH
    QML_FILES
        ui/Main.qml
        SOURCES src/backend.hpp src/backend.cpp
)

set_target_properties(filter-designer PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(filter-designer PRIVATE filter-designer-src Qt6::Quick Qt6::Charts)

add_subdirectory(test)
