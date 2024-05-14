#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include "RectangularWindow.hpp"
#include "BlackmanWindow.hpp"
#include <vector>

class FIRFilter {
public:
  FIRFilter(int cutoffFrequencyHz, int coefficientsCount,
            const BlackmanWindow &window, int samplingRateHz);

  int getCutoffFrequency() const;
  int getSamplingRate() const;
  const std::vector<double> &getFilterCoefficients() const;
  std::vector<double> calculateResponseDB(int fromFrequencyHz, int toFrequencyHz);

private:
  int cutoffFrequencyHz;
  const BlackmanWindow &window;
  int samplingRateHz;
  std::vector<double> filterCoefficients;

  std::vector<double> generateIdealFrequencyResponse() const;
  std::vector<double> calculateFilterCoefficients(int coefficientsCount) const;
};

#endif