#include "BlackmanWindow.hpp"
#include <cmath>

using namespace std;

/**
 * Blackman window.
 * Expected attenuation -74dB.
 *
 * @param windowSize
 * @return blackman window multipliers
 */
vector<double> BlackmanWindow::getCoefficients(const int windowSize) const {
  if (windowSize < 1) {
    throw invalid_argument("getCoefficients: windowSize must be >= 1");
  }
  
  vector<double> coefficients;
  coefficients.reserve(windowSize);

  for (int i = 0; i < windowSize; i++) {
    coefficients.push_back(0.42 - 0.5 * cos((2 * M_PI * i) / (windowSize - 1)) +
                           0.08 * cos((4 * M_PI * i) / (windowSize - 1)));
  }

  return coefficients;
}
