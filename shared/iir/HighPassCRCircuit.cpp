#include "HighPassCRCircuit.hpp"
#include "Capacitance.hpp"
#include <cmath>

using namespace std;

/**
 * Calcuate IIR high pass filter coefficients
 */
std::vector<double> HighPassCRCircuit::getFilterCoefficients() const {
  /*

   Simulate analog CR circuit to implement a high pass filter

     Vin --C--+-- Vout
              |
              R
              |
              g

    Vout = iR * R
    iR = iC
    iC = dQ / dt
    Q = C * (Vin - Vout)
    iC = C * (dVin / dt - dVout / dt)

    Discretising using backward Euler method:
    iC = C * (Vin[n] - Vin[n-1] - Vout[n] + Vout[n-1]) / T

    Vout = RC/T * (Vin[n] - Vin[n-1] - Vout[n] + Vout[n-1])
    Vout = RC/T * Vout[n] = RC/T * Vin[n] - RC/T * Vin[n-1] + RC/T * Vout[n-1]

    Given a is a filter coefficient:
    a = RC / (T + RC)
    Vout = a * Vin[n] - a * Vin[n-1] + a * Vout[n-1]
   */

    const double samplingTime{1 / (double)getSamplingRate()};
    const double crConstant{rcConstant(getCutoffFrequency())};
    const double a = crConstant / (samplingTime + crConstant);

    return std::vector<double>({a, -a, a});
}

