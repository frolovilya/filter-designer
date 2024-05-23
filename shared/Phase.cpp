#include "Phase.hpp"
#include <cmath>
#include <vector>
#include <numbers>

using namespace std;

/**
 * Convert to [-360, 360]
 */
double normalizeAngle(double angle) {
    angle = fmod(angle + numbers::pi, numbers::pi * 2);
  if (angle < 0) {
    angle += numbers::pi * 2;
  }
  angle = angle - numbers::pi;

  return std::fmod(angle, numbers::pi * 2);
}

/**
 * Diff between two angles in radians
 */
double angleDiff(double a, double b) {
  double diff = std::fmod(b - a + numbers::pi, numbers::pi * 2);
  if (diff < 0) {
    diff += numbers::pi * 2;
  }
  return diff - numbers::pi;
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
