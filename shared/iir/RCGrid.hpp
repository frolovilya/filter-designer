#ifndef RC_GRID_H
#define RC_GRID_H

#include "IIRFilterCoefficients.hpp"

class RCGrid {
public:
  RCGrid(int cutoffFrequency, int samplingRate);

  int getCutoffFrequency() const;
  int getSamplingRate() const;
  IIRFilterCoefficients getIIRFilterCoefficients() const;

private:
  const int cutoffFrequency;
  const int samplingRate;
  const IIRFilterCoefficients coefficients;

  double calculateRCConstant(int frequency) const;
  IIRFilterCoefficients
  calculateIIRFilterCoefficients(int frequency, int samplingRate) const;
};

#endif
