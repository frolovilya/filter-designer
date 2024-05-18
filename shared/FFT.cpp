#include "FFT.hpp"
#include "fftw3.h"
#include <vector>

using namespace std;

/**
 * Perform direct or inverse Fast Fourier Transform
 *
 * @param samples complex values buffer to perform FFT on
 * @param direct inverse or direct FFT
 * @return complex FFT result
 */
vector<complex<double>> fft::transform(const vector<complex<double>> &samples,
                                       bool direct) {
  fftw_complex *in =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  fftw_complex *out =
      (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());

  int direction = direct ? FFTW_FORWARD : FFTW_BACKWARD;
  fftw_plan p =
      fftw_plan_dft_1d(samples.size(), in, out, direction, FFTW_ESTIMATE);

  // initialize the input
  for (unsigned int i = 0; i < samples.size(); i++) {
    in[i][0] = samples[i].real();
    in[i][1] = samples[i].imag();
  }

  fftw_execute(p);

  vector<complex<double>> result;
  result.reserve(samples.size());

  for (unsigned int i = 0; i < samples.size(); i++) {
    result.push_back(complex<double>(out[i][0], out[i][1]));
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);

  return result;
}

vector<complex<double>> fft::direct(const vector<complex<double>> &samples) {
  return fft::transform(samples, true);
}

vector<complex<double>> fft::inverse(const vector<complex<double>> &samples) {
  return fft::transform(samples, false);
}

/**
 * Convert real values vector to complex values vector
 *
 * @param samples real sample values
 * @return complex(real, 0) samples
 */
vector<complex<double>> fft::toComplexVector(const vector<double> &samples) {
  vector<complex<double>> result;
  result.reserve(samples.size());

  for (const double &sample : samples) {
    result.push_back(complex<double>(sample));
  }

  return result;
}
