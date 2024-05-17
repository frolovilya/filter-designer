#include "RectangularWindow.hpp"

using namespace std;

/**
 * Rectangular window.
 * Basic window of 1's that doesn't alter coefficients.
 * Expected attenuation -21dB.
 *
 * @param windowSize
 * @return rectangular window multipliers
 */
vector<double> RectangularWindow::getCoefficients(int windowSize) const {
  if (windowSize < 1) {
    throw invalid_argument("getCoefficients: windowSize must be >= 1");
  }

  vector<double> coefficients;

  for (int i = 0; i < windowSize; i++) {
    coefficients.push_back(1);
  }

  return coefficients;
}
