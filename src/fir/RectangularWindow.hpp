#ifndef RECTANGULAR_WINDOW_H
#define RECTANGULAR_WINDOW_H

#include <vector>

class RectangularWindow {
public:
  std::vector<double> getCoefficients(int windowSize) const;
  std::vector<double> apply(const std::vector<double> &filterCoefficients) const;
};

#endif