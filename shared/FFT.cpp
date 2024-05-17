#include "FFT.hpp"
#include "fftw3.h"
#include <vector>

using namespace std;

/**
 * Perform direct or inverse Fast Fourier Transform on a given sample data
 *
 * @param samples buffer to perform FFT on
 * @param inverse inverse or direct FFT
 * @return complex FFT result
 */
vector<complex<double>> fft(const vector<complex<double>> &samples,
                            bool inverse) {
  fftw_complex *in, *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  int direction = inverse ? FFTW_BACKWARD : FFTW_FORWARD;
  p = fftw_plan_dft_1d(samples.size(), in, out, direction, FFTW_ESTIMATE);

  for (unsigned int i = 0; i < samples.size(); i++) {
    in[i][0] = samples[i].real();
    in[i][1] = samples[i].imag();
  }

  fftw_execute(p);

  vector<complex<double>> result;
  for (unsigned int i = 0; i < samples.size(); i++) {
    result.push_back(complex<double>(out[i][0], out[i][1]));
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return result;
}

vector<complex<double>> fftDirect(const vector<complex<double>> &samples) {
  return fft(samples, false);
}

vector<complex<double>> fftInverse(const vector<complex<double>> &samples) {
  return fft(samples, true);
}

/**
 * Convert real samples to complex vector
 *
 * @param samples real sample value
 * @return complex(real, 0) samples
 */
vector<std::complex<double>> toComplexVector(const vector<double> &samples) {
  vector<complex<double>> result;
  for (const double &sample : samples) {
    result.push_back(complex<double>(sample));
  }

  return result;
}
