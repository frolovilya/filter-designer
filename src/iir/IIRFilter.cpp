#include "IIRFilter.hpp"
#include "../Sampling.hpp"
#include "../SineWave.hpp"

using namespace std;

IIRFilter::IIRFilter(const RCGrid &rcGrid) : rcGrid{rcGrid} {}

const RCGrid &IIRFilter::getRCGrid() const { return rcGrid; }

vector<double> IIRFilter::calculateResponseDB(int fromFrequencyHz,
                                              int toFrequencyHz) const {
  if (fromFrequencyHz < 1) {
    throw invalid_argument("calculateResponseDb: fromFrequencyHz must be >= 1");
  }
  if (toFrequencyHz <= fromFrequencyHz) {
    throw invalid_argument(
        "calculateResponseDb: toFrequencyHz must be > fromFrequencyHz");
  }

  vector<double> frequencyResponse;
  auto sine = SineWave(rcGrid.getSamplingRate());
  for (int frequency = fromFrequencyHz; frequency < toFrequencyHz;
       frequency++) {
    auto samples = sine.generatePeriod(frequency, 1);
    auto filteredSamples = apply(samples);

    frequencyResponse.push_back(toDB(maxAbsValue(filteredSamples)));
  }

  return frequencyResponse;
}

vector<double> IIRFilter::apply(const vector<double> &samples) const {
  double vOutFeedback = 0;

  vector<double> result;
  for (const double &sample : samples) {
    // Given A and B are IIR filter coefficients:
    // Vout[n] = A * Vin[n] + B * Vout[n-1]
    vOutFeedback = rcGrid.getIIRFilterCoefficients().a * sample +
                   rcGrid.getIIRFilterCoefficients().b * vOutFeedback;

    result.push_back(vOutFeedback);
  }

  return result;
}
