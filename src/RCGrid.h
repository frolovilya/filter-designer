#include "IIRCoefficients.h"

/**
 *  Vin --R--+-- Vout
 *           |
 *           C
 *           |
 *           g
 */
class RCGrid {
public:
  RCGrid(int cutoffFrequencyHz);
  int getCutoffFrequency();
  IIRCoefficients calculateIIRFilterCoefficients(int samplingRateHz);

private:
  const int cutoffFrequencyHz;
  const double rcConstant;
  constexpr double calculateRCConstant(const double frequencyHz);
};