#include "Sampling.hpp"
#include <cmath>

/**
 * Nyquist frequency defines max possible frequency
 * to be digitally captured with a given sampling rate
 *
 * @param samplingRate sampling rate (Hz)
 * @return max possible frequency (Hz)
 */
int nyquistFrequency(const int samplingRate) { return samplingRate / 2; }

/**
 * Get max abs() vector value
 *
 * @param samples reference to a vector with samples
 * @return max abs value
 */
double maxAbsValue(const std::vector<double> &samples) {
  double maxValue = 0;
  for (const double &sample : samples) {
    if (abs(sample) > maxValue) {
      maxValue = abs(sample);
    }
  }

  return maxValue;
}

/**
 * Normalize values to [0..1] range
 *
 * @param values reference to a vector with values to normalize
 * @return vector with normalized values
 */
std::vector<double> normalize(const std::vector<double> &values) {
  double maxValue = maxAbsValue(values);

  std::vector<double> normalized;
  normalized.reserve(values.size());
  for (const double &v : values) {
    normalized.push_back(v / maxValue);
  }

  return normalized;
}

/**
 * Convert given value to dB
 *
 * @param value
 * @return dB
 */
double toDB(double value) {
  return 20 * log10(value);
}
