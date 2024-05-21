#include "FIRFilter.hpp"
#include "../FFT.hpp"
#include "../Sampling.hpp"
#include "../SineWave.hpp"
#include <cmath>

using namespace std;

/**
 * Finite Impulse Response filter
 */
FIRFilter::FIRFilter(FilterPass passType, int cutoffFrequency,
                     int coefficientsCount, const Window &window,
                     int samplingRate)
    : passType{passType}, cutoffFrequency{cutoffFrequency}, window{window},
      samplingRate{samplingRate} {
  if (cutoffFrequency < 1) {
    throw invalid_argument("FIRFilter: cutoffFrequency must be >= 1");
  }
  if (samplingRate < 1) {
    throw invalid_argument("FIRFilter: samplingRate must be >= 1");
  }
  filterCoefficients = calculateFilterCoefficients(coefficientsCount);
}

int FIRFilter::getCutoffFrequency() const { return cutoffFrequency; }

FilterPass FIRFilter::getPassType() const { return passType; }

int FIRFilter::getSamplingRate() const { return samplingRate; }

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
  response.reserve(samplingRate);

  // Using low-pass symmetric response to model all type of filters.
  // So that for a high pass filter need to calculate low pass with
  // modellingCutoffFrequency = samplingRate/2 - cutoffFrequency
  // to then shift the coefficients.
  const int modellingLowPassCutoffFrequency =
      passType == FilterPass::lowPass
          ? cutoffFrequency
          : nyquistFrequency(samplingRate) - cutoffFrequency;

  for (int i = 0; i < samplingRate; i++) {
    response.push_back(
        (i < modellingLowPassCutoffFrequency) ||
                (i >= samplingRate - modellingLowPassCutoffFrequency)
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

  // Since frequency response is symmetrical starting from samplingRate / 2,
  // concat the right side to the left side to get filter coefficients.
  // Skip 0 and last bins with DC values.
  //
  // If event coefficients count, then take equal number Ne=coefficientsCount/2
  // of values from each side.
  // If odd coefficients count, then No=Ne-1. Take No+1 from the left side and
  // No from the right side.
  const bool isEvenCount = coefficientsCount % 2 == 0;
  for (unsigned int i = filterTimeDomain.size() - coefficientsCount / 2 - 1;
       i < filterTimeDomain.size() - 1; i++) {
    coefficients.push_back(filterTimeDomain[i].real());
  }
  for (int i = 1; i <= coefficientsCount / 2 + (isEvenCount ? 0 : 1); i++) {
    coefficients.push_back(filterTimeDomain[i].real());
  }

  return normalize(window.apply(shiftFilterCoefficients(coefficients)));
}

/**
 * Low-pass filter response is used to design all other filter types
 * by shifting the coefficients to match pass band frequencies.
 *
 * Low pass ideal frequency response with cutoff frequency c and sampling rate
 * f:
 * {0}1111{c}0000{f/2}0000{f-c}1111{f}
 *
 * To covert low pass into high (or band pass in general),
 * need to shift it's ideal frequency response to f/2:
 * {0}0000{c}1111{f/2}0000{f-c}0000{f}1111{f+c}
 *
 * This is achieved by multiplying each filter coefficient by sine wave sampled
 * at f/2 frequency.
 *
 * @return
 */
vector<double> FIRFilter::shiftFilterCoefficients(
    const vector<double> &unshiftedCoefficients) const {

  vector<double> shiftedCoefficients(unshiftedCoefficients);

  if (passType == FilterPass::highPass) {
    SineWave sine = SineWave(samplingRate);
    // shift to Pi/4 to sample only high and low values, otherwise wave starts
    // from 0
    auto period = sine.generatePeriod(samplingRate / 2, 1, M_PI / 4);

    for (unsigned int i = 0; i < shiftedCoefficients.size(); i++) {
      shiftedCoefficients[i] *= period[i % period.size()];
    }
  }

  return shiftedCoefficients;
}

/**
 * Calculate FIR filter frequency response from 1 to samplingRate / 2
 *
 * @return magnitudes (dB) for each frequency
 */
vector<double> FIRFilter::calculateResponseDB() const {
  const int fromFrequency = 1;
  const int toFrequency = nyquistFrequency(samplingRate);

  vector<double> paddedCoefficients(filterCoefficients);
  for (int i = filterCoefficients.size(); i < samplingRate; i++) {
    paddedCoefficients.push_back(0);
  }

  auto fftResult = fft::direct(fft::toComplexVector(paddedCoefficients));
  vector<double> frequencyResponse;
  frequencyResponse.reserve(toFrequency - fromFrequency);

  for (int i = fromFrequency - 1; i < toFrequency; i++) {
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
