#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include "../Filter.hpp"
#include "../FilterPass.hpp"
#include "Window.hpp"
#include <vector>

class FIRFilter : public Filter {
public:
  FIRFilter(FilterPass passType, int cutoffFrequencyHz, int coefficientsCount,
            const Window &window, int samplingRateHz);

  int getCutoffFrequency() const override;
  int getSamplingRate() const override;

  std::vector<double> getFilterCoefficients() const override;
  std::vector<double> calculateResponseDB(int fromFrequencyHz,
                                          int toFrequencyHz) const override;

  std::vector<double> generateIdealFrequencyResponse() const;

  static int getOptimalCoefficientsCount(int samplingRate, double attenuationDB,
                                         int transitionLength);
  static int getTransitionLength(int samplingRate, double attenuationDB,
                                 int coefficientsCount);

private:
  const FilterPass passType;
  const int cutoffFrequencyHz;
  const Window &window;
  const int samplingRateHz;
  std::vector<double> filterCoefficients;

  std::vector<double> shiftFilterCoefficients(
      const std::vector<double> &unshiftedCoefficients) const;
  std::vector<double> calculateFilterCoefficients(int coefficientsCount) const;
};

#endif
