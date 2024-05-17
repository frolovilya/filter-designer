#include "SineWave.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

SineWave::SineWave(const int samplingRateHz) : samplingRateHz{samplingRateHz} {
  if (samplingRateHz < 1) {
    throw invalid_argument("SineWave: samplingRateHz must be >= 1");
  }
}

int SineWave::calculatePeriodSamplesCount(const int frequency) const {
  if (frequency < 1) {
    throw invalid_argument(
        "calculatePeriodSamplesCount: frequency must be >= 1");
  }
  if (frequency > samplingRateHz) {
      throw invalid_argument(
          "calculatePeriodSamplesCount: frequency must be < samplingRate");
  }
  // wave with a given frequency has a period T which takes this amount samples
  // per second
  return ceil(samplingRateHz / (double) frequency);
}

vector<double> SineWave::generatePeriod(const int frequency,
                                        const double amplitude) const {
  if (frequency < 1) {
    throw invalid_argument("generatePeriod: frequency must be >= 1");
  }
  if (amplitude < 0) {
    throw invalid_argument("generatePeriod: amplitude must be positive");
  }

  const int period = calculatePeriodSamplesCount(frequency);
  vector<double> samples;
  for (int i = 0; i < period; i++) {
    samples.push_back(sin(2 * M_PI * i / period) * amplitude);
  }

  return samples;
}
