#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include <vector>

class SineWave {
public:
  explicit SineWave(int samplingRate);
  
  std::vector<double> generatePeriod(int frequency, double amplitude, double phaseShift = 0) const;

private:
  const int samplingRate;

  int calculatePeriodSamplesCount(int frequency) const;
};

#endif
