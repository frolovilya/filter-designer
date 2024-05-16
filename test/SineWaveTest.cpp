#include "../shared/SineWave.hpp"
#include "../shared/FFT.hpp"
#include <boost/test/unit_test.hpp>
#include <complex>

using namespace std;

BOOST_AUTO_TEST_SUITE(SineWave_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(SineWave(0), invalid_argument);
  BOOST_REQUIRE_NO_THROW(SineWave(10));
}

BOOST_AUTO_TEST_CASE(generation_test) {

  const double amplitude = 1;
  const int waveFrequencyHz = 100;
  const int samplingRateHz = 20000;

  auto generator = SineWave(samplingRateHz);
  auto wave = generator.generatePeriod(waveFrequencyHz, amplitude);

  BOOST_TEST(wave.size() == samplingRateHz / waveFrequencyHz);

  double max = *max_element(wave.begin(), wave.end());
  BOOST_TEST(max == amplitude);

  double min = *min_element(wave.begin(), wave.end());
  BOOST_TEST(min == -amplitude);

  auto fftResult = fftDirect(toComplexVector(wave));
  int dominantFrequency = 1;
  // skip DC offset bin 0
  for (unsigned int i = 1; i < fftResult.size() / 2; i++) {
    if (abs(fftResult[i]) > abs(fftResult[dominantFrequency])) {
      dominantFrequency = i;
    }
  }
  // wave buffer contains one period samples
  BOOST_TEST(dominantFrequency == 1);
}

BOOST_AUTO_TEST_SUITE_END()
