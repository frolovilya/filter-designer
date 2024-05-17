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

BOOST_AUTO_TEST_CASE(frequency_less_than_samplingRate_test) {
  auto generator = SineWave(1000);
  BOOST_REQUIRE_THROW(generator.generatePeriod(1001, 1), invalid_argument);
}

void testWaveGeneration(int waveFrequency, int samplingRate, double amplitude) {
    auto generator = SineWave(samplingRate);
    auto wave = generator.generatePeriod(waveFrequency, amplitude);

    BOOST_TEST(wave.size() == ceil(samplingRate / (double) waveFrequency));

    const double tolerance = 0.1;
    double max = *max_element(wave.begin(), wave.end());
    BOOST_TEST(abs(amplitude - abs(max)) < tolerance);

    double min = *min_element(wave.begin(), wave.end());
    BOOST_TEST(abs(amplitude - abs(min)) < tolerance);

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

BOOST_AUTO_TEST_CASE(generation_test) {
  testWaveGeneration(10, 1000, 1);
  testWaveGeneration(440, 10000, 1);
  testWaveGeneration(800, 20000, 1);
  testWaveGeneration(10000, 48000, 1);
  testWaveGeneration(21000, 100000, 1);
}

BOOST_AUTO_TEST_SUITE_END()
