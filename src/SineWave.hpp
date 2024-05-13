#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include <vector>

class SineWave {
public:
  SineWave(int samplingRateHz);
  
  std::vector<double> generatePeriod(int frequency, double amplitude);

private:
  const int samplingRateHz;

  int calculatePeriodSamplesCount(int frequency);
};

#endif