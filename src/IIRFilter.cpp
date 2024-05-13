module;

#include "IIRFilterCoefficients.hpp"
#include <vector>

import RCGrid;

using namespace std;

export module IIRFilter;

export class IIRFilter {
public:
  IIRFilter(const RCGrid &rcGrid);
  int getSamplingRate() const;
  std::vector<double> apply(const std::vector<double> &samples);

private:
  const RCGrid &rcGrid;
  double vOutFeedback;

  double apply(double vIn);
};

IIRFilter::IIRFilter(const RCGrid &rcGrid) : rcGrid{rcGrid}, vOutFeedback{0} {}

int IIRFilter::getSamplingRate() const { return rcGrid.getSamplingRate(); }

double IIRFilter::apply(double vIn) {
  /*
  Given A and B are IIR filter coefficients:
  Vout[n] = A * Vin[n] + B * Vout[n-1]
  */
  vOutFeedback = rcGrid.getIIRFilterCoefficients().a * vIn +
                 rcGrid.getIIRFilterCoefficients().b * vOutFeedback;
  return vOutFeedback;
}

vector<double> IIRFilter::apply(const vector<double> &samples) {
  vOutFeedback = 0;

  vector<double> result;
  for (const double &sample : samples) {
    result.push_back(apply(sample));
  }

  return result;
}
