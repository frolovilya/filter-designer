#ifndef SAMPLING_H
#define SAMPLING_H

#include <vector>

double maxAbsValue(const std::vector<double> &samples);
std::vector<double> normalize(const std::vector<double> &samples);
int nyquistFrequency(int samplingRate);
double toDB(double value);

#endif