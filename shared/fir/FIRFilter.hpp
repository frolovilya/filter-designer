#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include "../Filter.hpp"
#include "Window.hpp"
#include <vector>

class FIRFilter : public Filter {
public:
  FIRFilter(int cutoffFrequencyHz, int coefficientsCount, const Window &window,
            int samplingRateHz);

  int getCutoffFrequency() const override;
  int getSamplingRate() const override;
  std::vector<double> getFilterCoefficients() const override;
  std::vector<double> calculateResponseDB(int fromFrequencyHz,
                                          int toFrequencyHz) const override;

private:
  const int cutoffFrequencyHz;
  const Window &window;
  const int samplingRateHz;
  std::vector<double> filterCoefficients;

  std::vector<double> generateIdealFrequencyResponse() const;
  std::vector<double> calculateFilterCoefficients(int coefficientsCount) const;
};

#endif
