#include "IIRFilter.hpp"
#include "../Sampling.hpp"
#include "Welle.hpp"

using namespace std;

/**
 * Infinite Impulse Response filter
 */
IIRFilter::IIRFilter(int cutoffFrequency, int samplingRate)
    : cutoffFrequency{cutoffFrequency}, samplingRate{samplingRate} {
  if (cutoffFrequency < 1) {
    throw std::invalid_argument("IIRFilter: cutoffFrequency must be >= 1");
  }
  if (samplingRate < 1) {
    throw std::invalid_argument("IIRFilter: samplingRate must be >= 1");
  }
  if (cutoffFrequency >= nyquistFrequency(samplingRate)) {
    throw std::invalid_argument("IIRFilter: cutoffFrequency must be < "
                                "samplingRate/2 (Nyquist frequency");
  }
}

int IIRFilter::getCutoffFrequency() const { return cutoffFrequency; }

int IIRFilter::getSamplingRate() const { return samplingRate; }

/**
 * Calculate IIR filter frequency response from 1 to samplingRate / 2
 *
 * @return magnitudes (dB) [-Inf, 0] and phase shifts [0] for each frequency
 */
vector<FilterResponse> IIRFilter::calculateResponse() const {
  const int fromFrequency = 1;
  const int toFrequency = nyquistFrequency(getSamplingRate());

  vector<FilterResponse> response;
  auto sine = welle::SineWave<double>(getSamplingRate());
  const double peakToPeakAmplitude = 2;
  for (int frequency = fromFrequency; frequency < toFrequency; frequency++) {
    auto samples = sine.generatePeriod(frequency, peakToPeakAmplitude);
    auto filteredSamples = apply(samples);

    response.push_back(FilterResponse(toDB(maxAbsValue(filteredSamples)),
                                      phaseShift(samples, filteredSamples)));
  }

  return response;
}

/**
 * Apply filter to a sample buffer.
 *
 * Using coefficients a,b,c returned by getFilterCoefficients() method:
 * Vout = a * Vin[n] + b * Vin[n-1] + c * Vout[n-1]
 *
 * @param samples input buffer
 * @return filtered samples
 */
vector<double> IIRFilter::apply(const vector<double> &samples) const {
  if (samples.size() == 0) {
    return samples;
  }

  const auto coefficients = getFilterCoefficients();
  if (coefficients.size() < 3) {
    throw std::logic_error("Expecting at least 3 IIR filter coefficients");
  }

  vector<double> result;
  result.push_back(samples[0]);
  for (unsigned int i = 1; i < samples.size(); i++) {
    // Vout = a * Vin[n] - a * Vin[n-1] + a * Vout[n-1]
    result.push_back(coefficients[0] * samples[i] +
                     coefficients[1] * samples[i - 1] +
                     coefficients[2] * result[i - 1]);
  }

  return result;
}
