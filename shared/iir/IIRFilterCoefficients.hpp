#ifndef IIR_FILTER_COEFFICIENTS_H
#define IIR_FILTER_COEFFICIENTS_H

struct IIRFilterCoefficients {
  IIRFilterCoefficients(double a, double b) : a{a}, b{b} {}

  const double a;
  const double b;
};

#endif