#include "LowPassRCCircuit.hpp"
#include "Capacitance.hpp"
#include <cmath>

using namespace std;

/**
 * Calcuate IIR low pass filter coefficients
 */
std::vector<double> LowPassRCCircuit::getFilterCoefficients() const {
  /*

  Simulate analog RC circuit to implement a low pass filter

    Vin --R--+-- Vout
             |
             C
             |
             g


   Vin = Vr + Vout
   Vr = iR * R

   Following the Kirchoff law:
   iR = iC
   iC = C * dVout / dt

   Vin = RC * dVout / dt + Vout

   Discretising using backward Euler method:
   Vin[n] = RC * (Vout[n] - Vout[n-1]) / T + Vout[n]

   Vin[n] = RC/T * Vout[n] - RC/T * Vout[n-1] + Vout[n]
   Vin[n] + RC/T * Vout[n-1] = Vout[n] * (RC/T + 1)
   Vout[n] = Vin[n] * 1 / (RC/T + 1) + Vout[n-1] * RC/T / (RC/T + 1)

   Then a and b are filter coefficients:
   a = 1 / (RC/T + 1)
   b = RC/T / (RC/T + 1)
  */

  const double samplingTime{1 / (double)getSamplingRate()};
  const double rct{rcConstant(getCutoffFrequency()) / samplingTime};
  const double a = 1 / (rct + 1);
  const double b = rct / (rct + 1);

  return vector<double>({a, 0, b});
}
