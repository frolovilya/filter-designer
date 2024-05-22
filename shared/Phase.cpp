#include "Phase.hpp"
#include <cmath>
#include <vector>

/**
 * Convert to [-360, 360]
 */
double normalizeAngle(double angle) {
  angle = std::fmod(angle + M_PI, M_PI * 2);
  if (angle < 0) {
    angle += M_PI * 2;
  }
  angle = angle - M_PI;

  return std::fmod(angle, M_PI * 2);
}

/**
 * Diff between two angles in radians
 */
double angleDiff(double a, double b) {
  double diff = std::fmod(b - a + M_PI, M_PI * 2);
  if (diff < 0) {
    diff += M_PI * 2;
  }
  return diff - M_PI;
}

/**
 * Unwrap radian phases by adding multiples of 2*pi as appropriate to
 * remove jumps greater than Pi
 */
std::vector<double> phaseUnwrap(const std::vector<double> &in) {
  std::vector<double> out;
  out.push_back(in[0]);

  for (unsigned int i = 1; i < in.size(); i++) {
    out.push_back(out[i - 1] - angleDiff(in[i], normalizeAngle(out[i - 1])));
  }

  return out;
}
