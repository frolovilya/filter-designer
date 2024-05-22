#ifndef FILTERRESPONSE_HPP
#define FILTERRESPONSE_HPP

#include "Phase.hpp"
#include <vector>

struct FilterResponse {
  const double magnitudeDB;
  const double phaseShift;

  FilterResponse(double m, double p) : magnitudeDB{m}, phaseShift{p} {}
};

inline std::vector<double>
magnitudes(const std::vector<FilterResponse> &filterResponse) {
  std::vector<double> result;
  result.reserve(filterResponse.size());
  for (auto const &r : filterResponse) {
    result.push_back(r.magnitudeDB);
  }

  return result;
}

inline std::vector<double>
phaseShifts(const std::vector<FilterResponse> &filterResponse) {
  std::vector<double> result;
  result.reserve(filterResponse.size());
  for (auto const &r : filterResponse) {
    result.push_back(r.phaseShift);
  }

  return phaseUnwrap(result);
}

#endif // FILTERRESPONSE_HPP
