#include "RCGrid.hpp"
#include "../Sampling.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

/**
 * Simulate analog RC grid to implement a low pass filter
 *
 *  Vin --R--+-- Vout
 *           |
 *           C
 *           |
 *           g
 */
RCGrid::RCGrid(const int cutoffFrequency, const int samplingRate)
    : cutoffFrequency{cutoffFrequency}, samplingRate{samplingRate},
      coefficients{
          calculateIIRFilterCoefficients(cutoffFrequency, samplingRate)} {
  if (cutoffFrequency < 1) {
    throw invalid_argument("RCGrid: cutoffFrequency must be >= 1");
  }
  if (samplingRate < 1) {
    throw invalid_argument("RCGrid: samplingRate must be >= 1");
  }
  if (cutoffFrequency >= nyquistFrequency(samplingRate)) {
      throw invalid_argument("RCGrid: cutoffFrequency must be < "
                             "samplingRate/2 (Nyquist frequency");
  }
}

int RCGrid::getCutoffFrequency() const { return cutoffFrequency; }

int RCGrid::getSamplingRate() const { return samplingRate; }

IIRFilterCoefficients RCGrid::getIIRFilterCoefficients() const {
  return coefficients;
}

/**
 * Calculate RC constant to be used for IIR coefficients calculation
 *
 * @param frequency desired cutoff frequency (must be >= 1) to achieve -3dB
 * attenuation
 * @return calculated RC constant
 */
double RCGrid::calculateRCConstant(const int frequency) const {
  if (frequency < 1) {
    throw invalid_argument("calculateRCConstant: frequency must be >= 1");
  }
  /*
  R = 1 / (2 * pi * frequency * C)
  RC = 1 / (2 * pi * frequency)
  */
  return 1 / (2 * M_PI * frequency);
}

/**
 * Calcuate IIR filter coefficients
 *
 * @param cutoffFrequency cutoffFrequency (must be >= 1)
 * @param samplingRate
 * @return IIR filter coefficients
 */
IIRFilterCoefficients
RCGrid::calculateIIRFilterCoefficients(const int cutoffFrequency,
                                       const int samplingRate) const {
  if (cutoffFrequency < 1) {
    throw invalid_argument(
        "calculateIIRFilterCoefficients: cutoffFrequency must be >= 1");
  }
  if (samplingRate < 1) {
    throw invalid_argument(
        "calculateIIRFilterCoefficients: samplingRate must be >= 1");
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

  const double samplingTime{1 / (double)samplingRate};
  const double rcConstant{calculateRCConstant(cutoffFrequency)};

  IIRFilterCoefficients coefficients(samplingTime / (samplingTime + rcConstant),
                                     rcConstant / (samplingTime + rcConstant));

  return coefficients;
}
