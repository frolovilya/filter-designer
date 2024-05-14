#include "RCGrid.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

RCGrid::RCGrid(const int cutoffFrequencyHz, const int samplingRateHz)
    : cutoffFrequencyHz{cutoffFrequencyHz}, samplingRateHz{samplingRateHz},
      coefficients{
          calculateIIRFilterCoefficients(cutoffFrequencyHz, samplingRateHz)} {
  if (cutoffFrequencyHz < 1) {
    throw invalid_argument("RCGrid: cutoffFrequencyHz must be >= 1");
  }
  if (samplingRateHz < 1) {
    throw invalid_argument("RCGrid: samplingRateHz must be >= 1");
  }
}

int RCGrid::getCutoffFrequency() const { return cutoffFrequencyHz; }

int RCGrid::getSamplingRate() const { return samplingRateHz; }

IIRFilterCoefficients RCGrid::getIIRFilterCoefficients() const {
  return coefficients;
}

double RCGrid::calculateRCConstant(const int frequencyHz) const {
  if (frequencyHz < 1) {
    throw invalid_argument("calculateRCConstant: frequencyHz must be >= 1");
  }
  /*
  R = 1 / (2 * pi * frequencyHz * C)
  RC = 1 / (2 * pi * frequencyHz)
  */
  return 1 / (2 * M_PI * frequencyHz);
}

IIRFilterCoefficients
RCGrid::calculateIIRFilterCoefficients(const int cutoffFrequencyHz,
                                       const int samplingRateHz) const {
  if (cutoffFrequencyHz < 1) {
    throw invalid_argument(
        "calculateIIRFilterCoefficients: cutoffFrequencyHz must be >= 1");
  }
  if (samplingRateHz < 1) {
    throw invalid_argument(
        "calculateIIRFilterCoefficients: samplingRateHz must be >= 1");
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

  Then A and B are IIR filter coefficients:
  A = T / (T + RC)
  B = RC / (T + RC)
  */

  const double samplingTime{1 / (double)samplingRateHz};
  const double rcConstant{calculateRCConstant(cutoffFrequencyHz)};

  IIRFilterCoefficients coefficients(samplingTime / (samplingTime + rcConstant),
                                     rcConstant / (samplingTime + rcConstant));

  return coefficients;
}