#include "../src/FFT.hpp"
#include "../src/Sampling.hpp"
#include "../src/SineWave.hpp"
#include <boost/test/unit_test.hpp>
#include <complex>

using namespace std;

BOOST_AUTO_TEST_SUITE(FFT_test)

BOOST_AUTO_TEST_CASE(direct_inverse_test) {
  const double amplitude = 1;
  const int waveFrequencyHz = 100;
  const int samplingRateHz = 20000;

  auto generator = SineWave(samplingRateHz);
  auto wave = generator.generatePeriod(waveFrequencyHz, amplitude);

  auto directResult = fftDirect(toComplexVector(wave));
  auto inverseResult = fftInverse(directResult);
  vector<double> restoredWave;
  for (unsigned int i = 0; i < inverseResult.size(); i++) {
    restoredWave.push_back(inverseResult[i].real());
  }

  restoredWave = normalize(restoredWave);

  BOOST_TEST(wave.size() == restoredWave.size());

  const double tolerance = 0.00000001;
  for (unsigned int i = 0; i < wave.size(); i++) {
    BOOST_TEST(abs(wave[i] - restoredWave[i]) < tolerance);
  }
}

BOOST_AUTO_TEST_SUITE_END()