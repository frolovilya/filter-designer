module;

#include <cmath>
#include <stdexcept>
#include <vector>

using namespace std;

export module SineWave;

export class SineWave {
public:
  SineWave(int samplingRateHz);
  std::vector<double> generatePeriod(int frequency, double amplitude);

private:
  const int samplingRateHz;
  int calculatePeriodSamplesCount(int frequency);
};

SineWave::SineWave(const int samplingRateHz) : samplingRateHz{samplingRateHz} {
  if (samplingRateHz < 1) {
    throw invalid_argument("SineWave: samplingRateHz must be >= 1");
  }
}

int SineWave::calculatePeriodSamplesCount(const int frequency) {
  if (frequency < 1) {
    throw invalid_argument(
        "calculatePeriodSamplesCount: frequency must be >= 1");
  }
  // wave with a given frequency has a period T which takes this amount samples
  // per second
  return samplingRateHz / frequency;
}

vector<double> SineWave::generatePeriod(const int frequency,
                                        const double amplitude) {
  if (frequency < 1) {
    throw invalid_argument("generatePeriod: frequency must be >= 1");
  }
  if (amplitude < 0) {
    throw invalid_argument("amplitude: amplitude must be positive");
  }

  const int period = calculatePeriodSamplesCount(frequency);
  vector<double> samples;
  for (int i = 0; i < period; i++) {
    samples.push_back(sin(2 * M_PI * i / period) * amplitude);
  }

  return samples;
}