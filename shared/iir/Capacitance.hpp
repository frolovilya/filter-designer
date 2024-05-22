#ifndef CAPACITANCE_HPP
#define CAPACITANCE_HPP

#include <cmath>

/**
 * Calculate CR constant to be used for IIR coefficients calculation
 *
 * @param cutoffFrequency target frequency with -3dB attenuation
 */
inline double rcConstant(int cutoffFrequency) {
  /*
    R = 1 / (2 * pi * frequency * C)
    RC = 1 / (2 * pi * frequency)
  */
  return 1 / (2 * M_PI * cutoffFrequency);
}

#endif
