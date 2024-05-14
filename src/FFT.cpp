#include "FFT.hpp"
#include "fftw3.h"
#include <vector>

using namespace std;

vector<complex<double>> fft(const vector<double> &samples, bool inverse) {
  fftw_complex *in, *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * samples.size());
  int direction = inverse ? FFTW_BACKWARD : FFTW_FORWARD;
  p = fftw_plan_dft_1d(samples.size(), in, out, direction, FFTW_ESTIMATE);

  for (unsigned int i = 0; i < samples.size(); i++) {
    in[i][0] = samples[i]; // real
    in[i][1] = 0;          // imaginary
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

vector<complex<double>> fftDirect(const vector<double> &samples) {
  return fft(samples, false);
}
vector<complex<double>> fftInverse(const vector<double> &samples) {
  return fft(samples, true);
}