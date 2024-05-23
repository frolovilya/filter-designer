#include "../shared/SineWave.hpp"
#include "../shared/FFT.hpp"
#include <boost/test/unit_test.hpp>
#include <complex>
#include <numbers>

using namespace std;

BOOST_AUTO_TEST_SUITE(SineWave_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(SineWave(0), invalid_argument);
  BOOST_REQUIRE_NO_THROW(SineWave(10));
}

BOOST_AUTO_TEST_CASE(frequency_less_than_nyquist_test) {
  const int samplingRate = 1000;
  const int frequency = 501;
  auto generator = SineWave(samplingRate);
  BOOST_REQUIRE_THROW(generator.generatePeriod(frequency, 1), invalid_argument);
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

    auto fftResult = fft::direct(fft::toComplexVector(wave));
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

void testSamplingWithPhaseShift(int waveFrequency, int samplingRate,
                                double phaseShift, double expectedFirstSampleValue) {
    auto generator = SineWave(samplingRate);
    auto wave = generator.generatePeriod(waveFrequency, 1, phaseShift);

    BOOST_TEST(wave.size() > 0);

    const double tolerance = 1e-5;
    BOOST_TEST(abs(abs(wave[0]) - abs(expectedFirstSampleValue)) < tolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_sampling_test) {
    // whole period is 2 * Pi
    testSamplingWithPhaseShift(10, 100, 0, 0);
    testSamplingWithPhaseShift(10, 100, numbers::pi / 2, 1);
    testSamplingWithPhaseShift(10, 100, numbers::pi, 0);
    testSamplingWithPhaseShift(10, 100, 3 * numbers::pi / 2, -1);
    testSamplingWithPhaseShift(10, 100, 2 * numbers::pi, 0);
}

void testPhaseShiftDiff(int waveFrequency, int samplingRate,
                        double phaseShift1, double phaseShift2,
                        double expectedDiff) {
    auto generator = SineWave(samplingRate);
    auto wave1 = generator.generatePeriod(waveFrequency, 1, phaseShift1);
    auto wave2 = generator.generatePeriod(waveFrequency, 1, phaseShift2);

    double actualDiff = SineWave::phaseShift(wave1, wave2);

    const double tolerance = 1e-5;
    BOOST_TEST(abs(actualDiff - expectedDiff) < tolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_diff_test) {
    testPhaseShiftDiff(440, 48000, 0, 0, 0);
    testPhaseShiftDiff(440, 48000, 0, numbers::pi, 0);
    testPhaseShiftDiff(440, 48000, 0, 2 * numbers::pi, 0);

    testPhaseShiftDiff(440, 48000, 0, numbers::pi / 2, numbers::pi / 2);
    testPhaseShiftDiff(440, 48000, 0, numbers::pi / 4, numbers::pi / 4);
    testPhaseShiftDiff(440, 48000, 0, 2 * numbers::pi / 3, 2 * numbers::pi / 3);
}

BOOST_AUTO_TEST_SUITE_END()
