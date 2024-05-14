#ifndef BLACKMAN_WINDOW_H
#define BLACKMAN_WINDOW_H

#include <vector>

class BlackmanWindow {
public:
  std::vector<double> getCoefficients(int windowSize) const;
  std::vector<double> apply(const std::vector<double> &filterCoefficients) const;
};

#endif