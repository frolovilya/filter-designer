#ifndef RC_GRID_H
#define RC_GRID_H

#include "IIRFilter.hpp"

class LowPassRCCircuit : public IIRFilter {
public:
  using IIRFilter::IIRFilter;

  std::vector<double> getFilterCoefficients() const override;
};

#endif
