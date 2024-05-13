#include "FIRFilter.hpp"
#include "fftw3.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

FIRFilter::FIRFilter(int cutoffFrequencyHz, int coefficientsCount,
                     const RectangularWindow &window, int samplingRateHz)
    : cutoffFrequencyHz{cutoffFrequencyHz},
      coefficientsCount{coefficientsCount}, window{window},
      samplingRateHz{samplingRateHz} {
  filterCoefficients = calculateFilterCoefficients();

  samplesBufferIndex = 0;
  for (int i = 0; i < coefficientsCount; i++) {
    samplesBuffer.push_back(0);
  }
}

int FIRFilter::getCutoffFrequency() const { return cutoffFrequencyHz; }

const RectangularWindow &FIRFilter::getWindow() const { return window; }

int FIRFilter::getSamplingRate() const { return samplingRateHz; }

const vector<double> &FIRFilter::getFilterCoefficients() const {
  return filterCoefficients;
}

vector<double> FIRFilter::fftReal(const vector<double> &samples,
                                  bool inverse) const {
  fftw_complex *in, *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  int direction = inverse ? FFTW_BACKWARD : FFTW_FORWARD;
  p = fftw_plan_dft_1d(samples.size(), in, out, direction, FFTW_ESTIMATE);

  for (unsigned int i = 0; i < samples.size(); i++) {
    in[i][0] = samples[i]; // real
    in[i][1] = 0;          // imaginary

    // cout << in[i][0] << " ";
  }
  // cout << "\n";

  fftw_execute(p);

  vector<double> result;
  for (unsigned int i = 0; i < samples.size(); i++) {
    result.push_back(out[i][0]);

    // cout << out[i][0] << " ";
  }
  // cout << "\n";

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return result;
}

int FIRFilter::nyquistFrequency() const { return samplingRateHz / 2; }

/**
 * Ideal low pass filter frequency response with 1 gain for pass band and 0 for
 * stop band
 */
vector<double> FIRFilter::generateIdealFrequencyResponse() const {
  int frequencyRangeHz = nyquistFrequency();

  vector<double> response;
  for (int i = 0; i < frequencyRangeHz; i++) {
    response.push_back(i <= cutoffFrequencyHz ? 1 : 0);
  }
  for (int i = frequencyRangeHz - 1; i >= 0; i--) {
    response.push_back(response[i]);
  }

  return response;
}

/**
 * Calculate filter coefficients for the given theoretical frequency response
 */
vector<double> FIRFilter::calculateFilterCoefficients() const {
  auto idealFrequencyResponse = generateIdealFrequencyResponse();
  vector<double> filterTimeDomain = fftReal(idealFrequencyResponse, true);

  vector<double> coefficients;
  double maxValue = 1;
  for (int i = coefficientsCount / 2; i > 1; i--) {
    coefficients.push_back(filterTimeDomain[i]);
    if (abs(filterTimeDomain[i]) > maxValue) {
      maxValue = abs(filterTimeDomain[i]);
    }
  }
  for (int i = 1; i < coefficientsCount / 2; i++) {
    coefficients.push_back(filterTimeDomain[i]);
    if (abs(filterTimeDomain[i]) > maxValue) {
      maxValue = abs(filterTimeDomain[i]);
    }
  }

  // normalize to [0..1] range
  for (double &value : coefficients) {
    value = value / maxValue;
  }

  return window.apply(coefficients);
}

double FIRFilter::apply(const double vIn) {
  samplesBuffer[samplesBufferIndex++] = vIn;
  samplesBufferIndex %= samplesBuffer.size();

  double filteredSample = 0;
  for (unsigned int i = samplesBufferIndex; i < samplesBufferIndex + samplesBuffer.size(); i++) {
    int j = i % samplesBuffer.size();

    filteredSample += samplesBuffer[j] * filterCoefficients[j];
  }

  return filteredSample;
}

vector<double> FIRFilter::apply(const vector<double> &samples) {
  vector<double> filtered;
  for (const double &sample: samples) {
    filtered.push_back(apply(sample));
  }
  
  return filtered;
}
