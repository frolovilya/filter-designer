cmake_minimum_required(VERSION 3.28)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-I/usr/local/opt/llvm/include")
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} 
    "-L/usr/local/opt/llvm@14/lib/c++ -Wl,-rpath,/usr/local/opt/llvm@14/lib/c++ -Wl,-syslibroot,/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX13.1.sdk")

add_compile_options(-Wall -Wextra -pedantic -Werror)

project(filter-designer)

add_library(modules)

target_sources(modules
  PUBLIC
    FILE_SET CXX_MODULES FILES
      src/SineWave.cpp
      src/IIRFIlter.cpp
      src/RCGrid.cpp
      src/FrequencyResponse.cpp
)

add_executable(${CMAKE_PROJECT_NAME} 
    src/main.cpp 
)

target_link_libraries(${CMAKE_PROJECT_NAME} modules)
