#include "RCGrid.h"
#include <cmath>
#include <stdexcept>

RCGrid::RCGrid(int cutoffFrequencyHz)
    : cutoffFrequencyHz{cutoffFrequencyHz},
      rcConstant{calculateRCConstant(cutoffFrequencyHz)} {}

int RCGrid::getCutoffFrequency() { return cutoffFrequencyHz; }

constexpr double RCGrid::calculateRCConstant(const double frequencyHz) {
  if (frequencyHz < 1) {
    throw std::invalid_argument("frequencyHz must be > 0");
  }
  /*
  R = 1 / (2 * pi * frequencyHz * C)
  RC = 1 / (2 * pi * frequencyHz)
  */
  return 1 / (2 * M_PI * frequencyHz);
}

IIRCoefficients RCGrid::calculateIIRFilterCoefficients(int samplingRateHz) {
  if (samplingRateHz < 1) {
    throw std::invalid_argument("samplingRateHz must be > 0");
  }
  /*
  Resistance:
  i = (V1 - V2) / R

  Capacitance:
  i = C * d(V1 - V2) / dt

  Following the Kirchoff law iC = iV (without R_out):
  (Vin - Vout) / R = C * dVout / dt
  Vout = Vin - RC * dVout / dt

  Discretising using backward Euler method:
  dV / dT ~= (V[n] - V[n-1]) / T
  Where T is a sampling time.

  Vout[n] = Vin[n] - RC * (Vout[n] - Vout[n-1]) / T
  T Vout[n] = T * Vin[n] - RC * Vout[n] + RC * Vout[n-1]
  Vout[n] (T + RC) = T * Vin[n] + RC * Vout[n-1]
  Vout[n] = T / (T + RC) * Vin[n] + RC / (T + RC) * Vout[n-1]

  Given A and B are IIR filter coefficients:
  Vout[n] = A * Vin[n] + B * Vout[n-1]
  */

  double samplingTime{1 / (double)samplingRateHz};

  IIRCoefficients coefficients(samplingTime / (samplingTime + rcConstant),
                               rcConstant / (samplingTime + rcConstant));

  return coefficients;
}