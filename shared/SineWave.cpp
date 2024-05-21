#include "SineWave.hpp"
#include "Sampling.hpp"
#include <cmath>
#include <stdexcept>

using namespace std;

SineWave::SineWave(const int samplingRate) : samplingRate{samplingRate} {
  if (samplingRate < 1) {
    throw invalid_argument("SineWave: samplingRate must be >= 1");
  }
}

/**
 * Calculate how many samples will a single-period sine wave with a given frequency
 * take with a given sampling rate
 *
 * @param frequency target wave frequency (must be >= 1 and <= Nyquist frequency)
 * @return number of samples to represent one period
 */
int SineWave::calculatePeriodSamplesCount(const int frequency) const {
  if (frequency < 1) {
    throw invalid_argument(
        "calculatePeriodSamplesCount: frequency must be >= 1");
  }
  if (frequency > nyquistFrequency(samplingRate)) {
      throw invalid_argument(
          "calculatePeriodSamplesCount: frequency must be < samplingRate / 2 (Nyquist frequency)");
  }

  return ceil(samplingRate / (double) frequency);
}

/**
 * Generate one period of a sine wave
 *
 * @param frequency target wave frequency (must be >= 1)
 * @param amplitude max wave amplitude (must be > 0)
 * @param phaseShift shift wave start
 * @return sine wave one period samples
 */
vector<double> SineWave::generatePeriod(const int frequency,
                                        const double amplitude,
                                        const double phaseShift) const {
  if (frequency < 1) {
    throw invalid_argument("generatePeriod: frequency must be >= 1");
  }
  if (amplitude < 0) {
    throw invalid_argument("generatePeriod: amplitude must be positive");
  }

  const int period = calculatePeriodSamplesCount(frequency);
  vector<double> samples;
  for (int i = 0; i < period; i++) {
    samples.push_back(sin(2 * M_PI * i / period + phaseShift) * amplitude);
  }

  return samples;
}
