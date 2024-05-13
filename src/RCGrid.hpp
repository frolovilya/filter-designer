#ifndef RC_GRID_H
#define RC_GRID_H

#include "IIRFilterCoefficients.hpp"

/**
 *  Vin --R--+-- Vout
 *           |
 *           C
 *           |
 *           g
 */
class RCGrid {
public:
  RCGrid(int cutoffFrequencyHz, int samplingRateHz);
  
  int getCutoffFrequency() const;
  int getSamplingRate() const;
  IIRFilterCoefficients getIIRFilterCoefficients() const;

private:
  const int cutoffFrequencyHz;
  const int samplingRateHz;
  const IIRFilterCoefficients coefficients;

  double calculateRCConstant(int frequencyHz);
  IIRFilterCoefficients calculateIIRFilterCoefficients(int frequencyHz,
                                                       int samplingRateHz);
};

#endif