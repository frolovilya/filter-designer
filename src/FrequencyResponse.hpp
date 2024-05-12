#ifndef FREQUENCY_RESPONSE_H
#define FREQUENCY_RESPONSE_H

#include "IIRFilter.hpp"
#include <vector>

class FrequencyResponse {
public:
  FrequencyResponse(IIRFilter &filter);
  std::vector<double> calculateResponseDb(int fromFrequencyHz,
                                          int toFrequencyHz);

private:
  IIRFilter &filter;
  double toDb(double value);
  double findMaxValue(const std::vector<double> &samples);
};

#endif