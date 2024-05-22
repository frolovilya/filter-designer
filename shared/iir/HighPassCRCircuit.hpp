#ifndef HIGHPASSCRCIRCUIT_H
#define HIGHPASSCRCIRCUIT_H

#include "IIRFilter.hpp"

class HighPassCRCircuit : public IIRFilter {
public:
  using IIRFilter::IIRFilter;

  std::vector<double> getFilterCoefficients() const override;
};

#endif
