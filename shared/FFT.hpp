#ifndef FFT_H
#define FFT_H

#include "fftw3.h"
#include <complex>
#include <vector>

namespace fft {

std::vector<std::complex<double>>
toComplexVector(const std::vector<double> &samples);

std::vector<std::complex<double>>
transform(const std::vector<std::complex<double>> &samples, bool direct = true);

std::vector<std::complex<double>>
direct(const std::vector<std::complex<double>> &samples);
std::vector<std::complex<double>>
inverse(const std::vector<std::complex<double>> &samples);

} // namespace fft

#endif
