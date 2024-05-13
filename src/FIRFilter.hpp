#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include "RectangularWindow.hpp"
#include <vector>

class FIRFilter {
public:
  FIRFilter(int cutoffFrequencyHz, int coefficientsCount,
            const RectangularWindow &window, int samplingRateHz);

  int getCutoffFrequency() const;
  const RectangularWindow &getWindow() const;
  int getSamplingRate() const;
  const std::vector<double> &getFilterCoefficients() const;
  std::vector<double> apply(const std::vector<double> &samples);

private:
  int cutoffFrequencyHz;
  int coefficientsCount;
  const RectangularWindow &window;
  int samplingRateHz;
  std::vector<double> filterCoefficients;
  int samplesBufferIndex;
  std::vector<double> samplesBuffer;

  int nyquistFrequency() const;
  std::vector<double> generateIdealFrequencyResponse() const;
  std::vector<double> calculateFilterCoefficients() const;
  std::vector<double> fftReal(const std::vector<double> &samples,
                              bool inverse = false) const;
  double apply(double vIn);
};

#endif