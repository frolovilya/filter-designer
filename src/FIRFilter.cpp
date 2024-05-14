#include "FIRFilter.hpp"
#include "FFT.hpp"
#include "Sampling.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

FIRFilter::FIRFilter(int cutoffFrequencyHz, int coefficientsCount,
                     const BlackmanWindow &window, int samplingRateHz)
    : cutoffFrequencyHz{cutoffFrequencyHz}, window{window},
      samplingRateHz{samplingRateHz} {
  filterCoefficients = calculateFilterCoefficients(coefficientsCount);
}

int FIRFilter::getCutoffFrequency() const { return cutoffFrequencyHz; }

int FIRFilter::getSamplingRate() const { return samplingRateHz; }

const vector<double> &FIRFilter::getFilterCoefficients() const {
  return filterCoefficients;
}

/**
 * Ideal low pass filter frequency response with 1 gain for pass band and 0 for
 * stop band
 */
vector<double> FIRFilter::generateIdealFrequencyResponse() const {
  int frequencyRangeHz = nyquistFrequency(samplingRateHz);

  vector<double> response;
  for (int i = 1; i < frequencyRangeHz; i++) {
    response.push_back(i <= cutoffFrequencyHz ? 1 : 0);
  }
  for (int i = frequencyRangeHz - 1; i >= 1; i--) {
    response.push_back(response[i]);
  }

  return response;
}

/**
 * Calculate filter coefficients for the given theoretical frequency response
 */
vector<double>
FIRFilter::calculateFilterCoefficients(int coefficientsCount) const {
  auto idealFrequencyResponse = generateIdealFrequencyResponse();
  vector<complex<double>> filterTimeDomain = fftInverse(idealFrequencyResponse);

  vector<double> coefficients;
  for (int i = coefficientsCount / 2 - 1; i >= 1; i--) {
    coefficients.push_back(filterTimeDomain[i].real());
  }
  for (int i = 1; i < coefficientsCount / 2; i++) {
    coefficients.push_back(filterTimeDomain[i].real());
  }

  return normalize(window.apply(coefficients));
}

vector<double> FIRFilter::calculateResponseDB(int fromFrequencyHz,
                                              int toFrequencyHz) {
  vector <double> paddedCoefficients(filterCoefficients);
  for (int i = filterCoefficients.size(); i < samplingRateHz / 2; i++) {
    paddedCoefficients.push_back(0);
  }

  auto fftResult = fftDirect(paddedCoefficients);
  vector<double> frequencyResponse;
  for (int i = fromFrequencyHz + 1; i < toFrequencyHz; i++) {
    frequencyResponse.push_back(abs(fftResult[i])); // Mod(complex)
  }
  frequencyResponse = normalize(frequencyResponse);
  for (double &value : frequencyResponse) {
    value = toDB(value);
  }

  return frequencyResponse;
}
