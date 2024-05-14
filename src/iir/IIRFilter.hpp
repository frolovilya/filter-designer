#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "IIRFilterCoefficients.hpp"
#include "RCGrid.hpp"
#include <vector>

class IIRFilter {
public:
  IIRFilter(const RCGrid &rcGrid);

  const RCGrid& getRCGrid() const;
  std::vector<double> calculateResponseDB(int fromFrequencyHz, int toFrequencyHz) const;

private:
  const RCGrid &rcGrid;

  std::vector<double> apply(const std::vector<double> &samples) const;
};

#endif