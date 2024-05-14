#include "Sampling.hpp"
#include <cmath>

int nyquistFrequency(int samplingRate) { return samplingRate / 2; }

double maxAbsValue(const std::vector<double> &samples) {
  double maxValue = 1;
  for (const double &sample : samples) {
    if (abs(sample) > maxValue) {
      maxValue = abs(sample);
    }
  }

  return maxValue;
}

std::vector<double> normalize(const std::vector<double> &samples) {
  double maxValue = maxAbsValue(samples);

  std::vector<double> normalized;
  for (const double &sample : samples) {
    normalized.push_back(sample / maxValue);
  }

  return normalized;
}

double toDB(double value) {
  return 20 * log10(abs(value));
}