#ifndef RECTANGULAR_WINDOW_H
#define RECTANGULAR_WINDOW_H

#include "Window.hpp"
#include <vector>

class RectangularWindow : public Window {
public:
  std::vector<double> getCoefficients(int windowSize) const override;
};

#endif