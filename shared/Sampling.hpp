#ifndef SAMPLING_H
#define SAMPLING_H

#include <vector>

double maxAbsValue(const std::vector<double> &samples);

std::vector<double> normalize(const std::vector<double> &samples);

int nyquistFrequency(const int samplingRate);

double toDB(double value);

double phaseShift(std::vector<double> wave1, std::vector<double> wave2);

#endif
