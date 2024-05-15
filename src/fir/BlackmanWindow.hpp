#ifndef BLACKMAN_WINDOW_H
#define BLACKMAN_WINDOW_H

#include "Window.hpp"
#include <vector>

class BlackmanWindow : public Window {
public:
  std::vector<double> getCoefficients(int windowSize) const override;
};

#endif