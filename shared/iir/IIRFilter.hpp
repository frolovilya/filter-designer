#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "../Filter.hpp"
#include <vector>

class IIRFilter : public Filter {
public:
  IIRFilter(int cutoffFrequency, int samplingRate);
  virtual ~IIRFilter() {}

  int getCutoffFrequency() const override;
  int getSamplingRate() const override;
  std::vector<FilterResponse> calculateResponse() const override;
  std::vector<double> apply(const std::vector<double> &samples) const;

private:
  const int cutoffFrequency;
  const int samplingRate;
};

#endif
