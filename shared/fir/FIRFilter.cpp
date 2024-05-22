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
  if (cutoffFrequency >= nyquistFrequency(samplingRate)) {
    throw invalid_argument("FIRFilter: cutoffFrequency must be < "
                           "samplingRate/2 (Nyquist frequency");
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

  // For instance, for a cutoff frequency C and sampling rate F the
  // following ranges must be set to 1:
  // [0..C) ((F-C)..F)
  for (int i = 0; i < samplingRate; i++) {
    response.push_back(
        (i < modellingLowPassCutoffFrequency) ||
                (i > samplingRate - modellingLowPassCutoffFrequency)
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
 * @return normalized [-1, 1] filter coefficients with applied window
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

  // Frequency response is symmetrical starting from samplingRate / 2.
  //
  // For even number of coefficient Ne=coefficientsCount/2,
  // concat [Ne .. 0) and [0 .. Ne)
  // For odd number of coefficient No=Ne-1
  // concat [No .. 0) and [0 .. No]
  const bool isEvenCount = coefficientsCount % 2 == 0;

  for (int i = coefficientsCount / 2; i > 0; i--) {
    coefficients.push_back(filterTimeDomain[i].real());
  }
  for (int i = 0; i <= coefficientsCount / 2 - (isEvenCount ? 1 : 0); i++) {
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
 * @return low-pass filter coefficients with applied shift to model high or band
 * pass filters
 */
vector<double> FIRFilter::shiftFilterCoefficients(
    const vector<double> &unshiftedCoefficients) const {

  vector<double> shiftedCoefficients(unshiftedCoefficients);

  if (passType == FilterPass::highPass) {
    SineWave sine = SineWave(samplingRate);
    // shift to Pi/2 to sample only high and low values, otherwise wave starts
    // from 0
    auto period = sine.generatePeriod(samplingRate / 2, 1, M_PI / 2);

    for (unsigned int i = 0; i < shiftedCoefficients.size(); i++) {
      shiftedCoefficients[i] *= period[i % period.size()];
    }
  }

  return shiftedCoefficients;
}

/**
 * Calculate FIR filter frequency response from 1 to samplingRate / 2
 *
 * @return magnitudes (dB) [-Inf, 0] and phase shifts for each frequency
 */
vector<FilterResponse> FIRFilter::calculateResponse() const {
  const int fromFrequency = 1;
  const int toFrequency = nyquistFrequency(samplingRate);

  vector<double> paddedCoefficients(filterCoefficients);
  for (int i = filterCoefficients.size(); i < samplingRate; i++) {
    paddedCoefficients.push_back(0);
  }

  auto fftResult = fft::direct(fft::toComplexVector(paddedCoefficients));
  vector<double> magnitudes;
  vector<double> phaseShifts;
  magnitudes.reserve(toFrequency);
  phaseShifts.reserve(toFrequency);

  for (int i = fromFrequency - 1; i < toFrequency; i++) {
    magnitudes.push_back(abs(fftResult[i]));
    phaseShifts.push_back(arg(fftResult[i]));
  }
  magnitudes = normalize(magnitudes);
  for (double &value : magnitudes) {
    value = toDB(abs(value));
  }

  vector<FilterResponse> response;
  response.reserve(magnitudes.size());
  for (unsigned int i = 0; i < magnitudes.size(); i++) {
      response.push_back(FilterResponse(magnitudes[i], phaseShifts[i]));
  }

  return response;
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
  int count = ceil(attenuationDB * nyquistFrequency(samplingRate) /
                   (22 * transitionLength));
  // return odd number of coefficients to have a linear phase characteristics
  return count % 2 == 0 ? count + 1 : count;
}
