#ifndef SINE_WAVE_H
#define SINE_WAVE_H

#include <vector>

class SineWave {
public:
  explicit SineWave(int samplingRate);
  
  std::vector<double> generatePeriod(int frequency, double amplitude, double phaseShift = 0) const;

  static double phaseShift(std::vector<double> wave1, std::vector<double> wave2);

private:
  const int samplingRate;

  int calculatePeriodSamplesCount(int frequency) const;
};

#endif
