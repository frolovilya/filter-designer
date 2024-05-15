#include "RectangularWindow.hpp"

using namespace std;

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
