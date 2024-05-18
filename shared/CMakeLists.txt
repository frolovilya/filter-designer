add_library(filter-designer-shared
  iir/RCGrid.cpp iir/RCGrid.hpp
  iir/IIRFilter.cpp iir/IIRFilter.hpp
  SineWave.cpp SineWave.hpp
  fir/FIRFilter.cpp fir/FIRFilter.hpp
  fir/Window.cpp fir/Window.hpp
  fir/RectangularWindow.cpp fir/RectangularWindow.hpp
  fir/BlackmanWindow.cpp fir/BlackmanWindow.hpp
  FFT.cpp FFT.hpp
  Sampling.cpp Sampling.hpp
  Filter.hpp
)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
target_include_directories(filter-designer-shared PUBLIC ${FFTW_HEADER_PATH})
target_link_libraries(filter-designer-shared ${FFTW_LIB_PATH})
