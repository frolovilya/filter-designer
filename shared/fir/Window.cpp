#include "Window.hpp"
#include <vector>

/**
 * Apply window to given filter coefficients
 * W[n] * C[n]
 *
 * @param filterCoefficients coefficients to apply window to
 * @return filtered coefficients
 */
std::vector<double> Window::apply(const std::vector<double> &filterCoefficients) const {
  auto windowCoefficients = getCoefficients(filterCoefficients.size());

  std::vector<double> result;
  for (unsigned int i = 0; i < filterCoefficients.size(); i++) {
    result.push_back(filterCoefficients[i] * windowCoefficients[i]);
  }

  return result;
}
