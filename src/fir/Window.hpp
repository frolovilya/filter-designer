#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

class Window {
public:
  virtual std::vector<double> getCoefficients(int windowSize) const = 0;
  std::vector<double> apply(const std::vector<double> &filterCoefficients) const;
};

#endif