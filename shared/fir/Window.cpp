#include "Window.hpp"
#include <vector>

std::vector<double> Window::apply(const std::vector<double> &filterCoefficients) const {
  auto windowCoefficients = getCoefficients(filterCoefficients.size());

  std::vector<double> result;
  for (unsigned int i = 0; i < filterCoefficients.size(); i++) {
    result.push_back(filterCoefficients[i] * windowCoefficients[i]);
  }

  return result;
}