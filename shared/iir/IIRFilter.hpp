#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include "../Filter.hpp"
#include "IIRFilterCoefficients.hpp"
#include "RCGrid.hpp"
#include <vector>

class IIRFilter : public Filter {
public:
  IIRFilter(const RCGrid &rcGrid);

  int getCutoffFrequency() const override;
  int getSamplingRate() const override;
  std::vector<double> getFilterCoefficients() const override;
  std::vector<double> calculateResponseDB(int fromFrequencyHz,
                                          int toFrequencyHz) const override;

private:
  const RCGrid &rcGrid;

  std::vector<double> apply(const std::vector<double> &samples) const;
};

#endif
