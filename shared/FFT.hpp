#ifndef FFT_H
#define FFT_H

#include <complex>
#include <vector>

std::vector<std::complex<double>>
fft(const std::vector<std::complex<double>> &samples, bool inverse = false);

std::vector<std::complex<double>>
fftDirect(const std::vector<std::complex<double>> &samples);

std::vector<std::complex<double>>
fftInverse(const std::vector<std::complex<double>> &samples);

std::vector<std::complex<double>> toComplexVector(const std::vector<double>& samples);

#endif