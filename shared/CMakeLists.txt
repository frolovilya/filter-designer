add_library(filter-designer-shared
  iir/RCGrid.cpp
  iir/IIRFilter.cpp
  SineWave.cpp
  fir/FIRFilter.cpp
  fir/Window.cpp
  fir/RectangularWindow.cpp
  fir/BlackmanWindow.cpp
  FFT.cpp
  Sampling.cpp
)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
target_include_directories(filter-designer-shared PRIVATE ${FFTW_HEADER_PATH})
target_link_libraries(filter-designer-shared ${FFTW_LIB_PATH})
