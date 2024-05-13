module;

#include <cmath>
#include <stdexcept>
#include <vector>

import SineWave;
import IIRFilter;

using namespace std;

export module FrequencyResponse;

export class FrequencyResponse {
public:
  FrequencyResponse(IIRFilter &filter);
  std::vector<double> calculateResponseDb(int fromFrequencyHz,
                                          int toFrequencyHz);

private:
  IIRFilter &filter;
  double toDb(double value);
  double findMaxValue(const std::vector<double> &samples);
};

FrequencyResponse::FrequencyResponse(IIRFilter &filter) : filter{filter} {}

double FrequencyResponse::toDb(double value) { return 20 * log10(value); }

double FrequencyResponse::findMaxValue(const vector<double> &samples) {
  double max = 0;
  for (const double &sample : samples) {
    if (sample > max) {
      max = sample;
    }
  }

  return max;
}

vector<double> FrequencyResponse::calculateResponseDb(int fromFrequencyHz,
                                                      int toFrequencyHz) {
  if (fromFrequencyHz < 1) {
    throw invalid_argument("calculateResponseDb: fromFrequencyHz must be >= 1");
  }
  if (toFrequencyHz <= fromFrequencyHz) {
    throw invalid_argument(
        "calculateResponseDb: toFrequencyHz must be > fromFrequencyHz");
  }

  vector<double> result;
  auto sine = SineWave(filter.getSamplingRate());
  for (int frequency = fromFrequencyHz; frequency < toFrequencyHz;
       frequency++) {
    auto samples = sine.generatePeriod(frequency, 1);
    auto filteredSamples = filter.apply(samples);

    result.push_back(toDb(findMaxValue(filteredSamples)));
  }

  return result;
}
