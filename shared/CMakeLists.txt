set(SHARED_LIB_NAME "FilterDesignerShared")

add_library(${SHARED_LIB_NAME}
  iir/LowPassRCCircuit.cpp iir/LowPassRCCircuit.hpp
  iir/IIRFilter.cpp iir/IIRFilter.hpp
  fir/FIRFilter.cpp fir/FIRFilter.hpp
  fir/Window.cpp fir/Window.hpp
  fir/RectangularWindow.cpp fir/RectangularWindow.hpp
  fir/BlackmanWindow.cpp fir/BlackmanWindow.hpp
  FFT.cpp FFT.hpp
  Sampling.cpp Sampling.hpp
  Filter.hpp
  FilterPass.hpp
  FilterResponse.hpp
  Phase.hpp
  Phase.cpp
  iir/HighPassCRCircuit.hpp iir/HighPassCRCircuit.cpp
  iir/Capacitance.hpp
)

find_path(FFTW_HEADER_PATH fftw3.h)
find_library(FFTW_LIB_PATH fftw3)
message("-- FFTW3 Library: " ${FFTW_LIB_PATH})

find_path(WELLE_HEADER_PATH Welle.hpp)
message("-- Welle Header: " ${WELLE_HEADER_PATH})

target_include_directories(${SHARED_LIB_NAME} PUBLIC ${FFTW_HEADER_PATH} ${WELLE_HEADER_PATH})
target_link_libraries(${SHARED_LIB_NAME} ${FFTW_LIB_PATH})
