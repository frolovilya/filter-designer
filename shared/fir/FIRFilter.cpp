#include "FIRFilter.hpp"
#include "../FFT.hpp"
#include "../Sampling.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

/**
 * Finite Impulse Response filter
 */
FIRFilter::FIRFilter(int cutoffFrequencyHz, int coefficientsCount,
                     const Window &window, int samplingRateHz)
    : cutoffFrequencyHz{cutoffFrequencyHz}, window{window},
      samplingRateHz{samplingRateHz} {
  if (cutoffFrequencyHz < 1) {
    throw invalid_argument("FIRFilter: cutoffFrequencyHz must be >= 1");
  }
  if (samplingRateHz < 1) {
    throw invalid_argument("FIRFilter: samplingRateHz must be >= 1");
  }
  filterCoefficients = calculateFilterCoefficients(coefficientsCount);
}

int FIRFilter::getCutoffFrequency() const { return cutoffFrequencyHz; }

int FIRFilter::getSamplingRate() const { return samplingRateHz; }

vector<double> FIRFilter::getFilterCoefficients() const {
  return filterCoefficients;
}

/**
 * Ideal low pass filter frequency response with 1 gain for pass band and 0 for
 * stop band
 *
 * @return ideal response magnitudes
 */
vector<double> FIRFilter::generateIdealFrequencyResponse() const {
  vector<double> response;
  response.reserve(samplingRateHz);

  // generate symmetric response
  for (int i = 0; i < samplingRateHz; i++) {
    response.push_back((i < cutoffFrequencyHz) ||
                               (i >= samplingRateHz - cutoffFrequencyHz)
                           ? 1
                           : 0);
  }

  return response;
}

/**
 * Calculate filter coefficients for the given theoretical frequency response.
 * Coefficients must be convolved with an input sample buffer.
 *
 * @param coefficientsCount target number of coefficients
 * @return filter coefficients with applied window
 */
vector<double>
FIRFilter::calculateFilterCoefficients(int coefficientsCount) const {
  if (coefficientsCount < 1) {
    throw invalid_argument(
        "calculateFilterCoefficients: coefficientsCount must be >= 1");
  }

  auto idealFrequencyResponse = generateIdealFrequencyResponse();
  vector<complex<double>> filterTimeDomain =
      fft::inverse(fft::toComplexVector(idealFrequencyResponse));

  vector<double> coefficients;
  coefficients.reserve(coefficientsCount);

  for (int i = coefficientsCount / 2; i >= 1; i--) {
    coefficients.push_back(filterTimeDomain[i].real());
  }
  for (int i = 1; i < coefficientsCount / 2 + 1; i++) {
    coefficients.push_back(filterTimeDomain[i].real());
  }

  return normalize(window.apply(coefficients));
}

/**
 * Calculate FIR filter frequency response
 *
 * @param fromFrequencyHz start frequency
 * @param toFrequencyHz end frequency
 * @return magnitudes (dB) for each frequency from fromFrequencyHz (index 0) to
 * toFrequencyHz
 */
vector<double> FIRFilter::calculateResponseDB(int fromFrequencyHz,
                                              int toFrequencyHz) const {
  if (fromFrequencyHz < 1) {
    throw invalid_argument("calculateResponseDb: fromFrequencyHz must be >= 1");
  }
  if (toFrequencyHz <= fromFrequencyHz) {
    throw invalid_argument(
        "calculateResponseDb: toFrequencyHz must be > fromFrequencyHz");
  }

  vector<double> paddedCoefficients(filterCoefficients);
  for (int i = filterCoefficients.size(); i < samplingRateHz; i++) {
    paddedCoefficients.push_back(0);
  }

  auto fftResult = fft::direct(fft::toComplexVector(paddedCoefficients));
  vector<double> frequencyResponse;
  frequencyResponse.reserve(toFrequencyHz - fromFrequencyHz);

  for (int i = fromFrequencyHz - 1; i < toFrequencyHz; i++) {
    frequencyResponse.push_back(abs(fftResult[i])); // Mod(complex)
  }
  frequencyResponse = normalize(frequencyResponse);
  for (double &value : frequencyResponse) {
    value = toDB(abs(value));
  }

  return frequencyResponse;
}

/**
 * Get transition length using Fred Harris "rule of thumb" formula
 * to achieve a desired attenuation
 *
 * @param attenuationDB desired filter attenuation in dB
 * @return frequencies transition length
 */
int FIRFilter::getTransitionLength(int samplingRate, double attenuationDB,
                                   int coefficientsCount) {
  return ceil(attenuationDB * nyquistFrequency(samplingRate) /
              (22 * coefficientsCount));
}

/**
 * Get optimal coefficients countusing Fred Harris "rule of thumb" formula
 * to achieve a desired attenuation
 *
 * @param attenuationDB desired filter attenuation in dB
 * @return frequencies transition length
 */
int FIRFilter::getOptimalCoefficientsCount(int samplingRate,
                                           double attenuationDB,
                                           int transitionLength) {
  return ceil(attenuationDB * nyquistFrequency(samplingRate) /
              (22 * transitionLength));
}
