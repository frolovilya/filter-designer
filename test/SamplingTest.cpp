#include "../shared/Sampling.hpp"
#include "Welle.hpp"
#include <boost/test/unit_test.hpp>
#include <numbers>

BOOST_AUTO_TEST_SUITE(Sampling_test)

BOOST_AUTO_TEST_CASE(nyquistFrequency_test) {
  BOOST_TEST(nyquistFrequency(0) == 0);
  BOOST_TEST(nyquistFrequency(48000) == 24000);
  BOOST_TEST(nyquistFrequency(9) == 4);
  BOOST_TEST(nyquistFrequency(-100) == -50);
}

BOOST_AUTO_TEST_CASE(maxAbsValue_test) {
  std::vector<double> samples;
  for (int i = -15; i <= 10; i++) {
    samples.push_back(i);
  }

  BOOST_TEST(maxAbsValue(samples) == 15);
}

BOOST_AUTO_TEST_CASE(normalize_test) {
  std::vector<double> samples;
  for (int i = -10; i <= 10; i++) {
    samples.push_back(i);
  }

  std::vector<double> expectedNormalized;
  for (const double &sample : samples) {
    expectedNormalized.push_back(sample / 10);
  }

  auto actualNormalized = normalize(samples);

  BOOST_TEST(actualNormalized == expectedNormalized);
}

BOOST_AUTO_TEST_CASE(toDB_test) {
  BOOST_TEST(toDB(0) == -std::numeric_limits<double>::infinity());
  BOOST_TEST(toDB(0.0001) == -80);
  BOOST_TEST(toDB(0.001) == -60);
  BOOST_TEST(toDB(0.01) == -40);
  BOOST_TEST(toDB(0.1) == -20);
  BOOST_TEST(toDB(1) == 0);
  BOOST_TEST(toDB(10) == 20);
  BOOST_TEST(toDB(100) == 40);
  BOOST_TEST(toDB(1000) == 60);
}

void testPhaseShiftDiff(int waveFrequency, int samplingRate, double phaseShift1,
                        double phaseShift2, double expectedDiff) {
  auto generator = welle::SineWave<double>(samplingRate);
  const double peakToPeakAmplitude = 2;
  auto wave1 =
      generator.generatePeriod(waveFrequency, peakToPeakAmplitude, phaseShift1);
  auto wave2 =
      generator.generatePeriod(waveFrequency, peakToPeakAmplitude, phaseShift2);

  double actualDiff = phaseShift(wave1, wave2);

  const double tolerance = 1e-5;
  BOOST_TEST(abs(abs(actualDiff) - abs(expectedDiff)) < tolerance);
}

BOOST_AUTO_TEST_CASE(phase_shift_diff_test) {
  testPhaseShiftDiff(440, 48000, 0, 0, 0);
  testPhaseShiftDiff(440, 48000, 0, std::numbers::pi, 0);
  testPhaseShiftDiff(440, 48000, 0, 2 * std::numbers::pi, 0);

  testPhaseShiftDiff(440, 48000, 0, std::numbers::pi / 2, std::numbers::pi / 2);
  testPhaseShiftDiff(440, 48000, 0, std::numbers::pi / 4, std::numbers::pi / 4);
  testPhaseShiftDiff(440, 48000, 0, 2 * std::numbers::pi / 3,
                     2 * std::numbers::pi / 3);
}

BOOST_AUTO_TEST_SUITE_END()
