#ifndef UNWRAP_HPP
#define UNWRAP_HPP

#include <cmath>
#include <vector>

/**
 * Normalize to [-180, 180)
 */
double constrainAngle(double x) {
    x = std::fmod(x + M_PI, M_PI * 2);
    if (x < 0)
        x += M_PI * 2;
    return x - M_PI;
}

/**
 * Convert to [-360, 360]
 */
double angleConv(double angle) {
    return std::fmod(constrainAngle(angle), M_PI * 2);
}

double angleDiff(double a, double b) {
    double diff = std::fmod(b - a + M_PI, M_PI * 2);
    if (diff < 0)
        diff += M_PI * 2;
    return diff - M_PI;
}

double unwrap(double previousAngle, double newAngle) {
    return previousAngle - angleDiff(newAngle, angleConv(previousAngle));
}

std::vector<double> phaseUnwrap(const std::vector<double>& in) {
    std::vector<double> out;
    out.push_back(in[0]);

    for (unsigned int i = 1; i < in.size(); i++) {
        out.push_back(unwrap(out[i-1], in[i]));
    }

    return out;
}

#endif // UNWRAP_HPP
