#include "../../shared/fir/FIRFilter.hpp"
#include "../../shared/fir/BlackmanWindow.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>

using namespace std;

BOOST_AUTO_TEST_SUITE(FIRFilter_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(
      FIRFilter(FilterPass::lowPass, -1, 100, BlackmanWindow(), 1000),
      invalid_argument);
  BOOST_REQUIRE_THROW(
      FIRFilter(FilterPass::lowPass, 100, -1, BlackmanWindow(), 1000),
      invalid_argument);
  BOOST_REQUIRE_THROW(
      FIRFilter(FilterPass::lowPass, 100, 100, BlackmanWindow(), -1),
      invalid_argument);
  BOOST_REQUIRE_THROW(
      FIRFilter(FilterPass::lowPass, 24000, 200, BlackmanWindow(), 48000),
      invalid_argument);
  BOOST_REQUIRE_NO_THROW(
      FIRFilter(FilterPass::lowPass, 23999, 200, BlackmanWindow(), 48000));
}

void idealFrequencyResponseTest(FilterPass pass, int cutoffFrequency,
                                int samplingRate, int filterSize) {
  FIRFilter filter = FIRFilter(pass, cutoffFrequency, filterSize,
                               BlackmanWindow(), samplingRate);
  auto response = filter.generateIdealFrequencyResponse();

  // ensure that ideal response is symmetric around center element
  const double tolerance = 10e-7;
  const int modellingLowPassFrequency =
      pass == FilterPass::lowPass ? cutoffFrequency
                                  : samplingRate / 2 - cutoffFrequency;

  for (int i = 0, j = response.size() - 1; i < j; i++, j--) {
    if (i == modellingLowPassFrequency - 1) {
      BOOST_TEST(response[i] == 1);
      BOOST_TEST(response[j] == 0);
    } else {
      BOOST_TEST(abs(response[i] - response[j]) < tolerance);
    }
  }
}

BOOST_AUTO_TEST_CASE(ideal_frequency_response_test) {
  idealFrequencyResponseTest(FilterPass::lowPass, 20, 100, 20);
  idealFrequencyResponseTest(FilterPass::lowPass, 1500, 21000, 373);
  idealFrequencyResponseTest(FilterPass::lowPass, 2000, 48000, 500);
  idealFrequencyResponseTest(FilterPass::highPass, 10000, 48000, 501);
  idealFrequencyResponseTest(FilterPass::highPass, 20000, 48000, 333);
  idealFrequencyResponseTest(FilterPass::highPass, 7000, 15000, 440);
}

void actualFrequencyResponseTest(FilterPass pass, int cutoffFrequency,
                                 int samplingRate, int filterSize) {
  cout << "FIR Response type=" << (pass == FilterPass::lowPass ? "low" : "high")
       << ", cutoff=" << cutoffFrequency << ", samplingRate=" << samplingRate
       << ", filterSize=" << filterSize << "\n";

  FIRFilter filter = FIRFilter(pass, cutoffFrequency, filterSize,
                               BlackmanWindow(), samplingRate);

  auto coefficients = filter.getFilterCoefficients();
  BOOST_TEST(coefficients.size() == filterSize);

  for (const double &c : coefficients) {
    // ensure there're no Inf or NaN coefficients
    BOOST_TEST_REQUIRE(std::isfinite(c));
    // check that coefficients are normalized
    BOOST_TEST((c >= -1 && c <= 1));
  }

  auto filterResponse = filter.calculateResponseDB();
  const int transitionPeriod = 500;
  const double attenuationDB = -10;
  // rought test that attenuation is -10dB at cutoff frequency + transition
  // period
  BOOST_TEST(filterResponse[cutoffFrequency + transitionPeriod] <
             -attenuationDB);
  for (const double &f : filterResponse) {
    // test that response magnitudes are negative
    BOOST_TEST(f <= 0);
  }
}

BOOST_AUTO_TEST_CASE(response_test) {
  // test normal frequencies
  for (int cutoffFrequency = 40; cutoffFrequency < 23999;
       cutoffFrequency += 800) {
    actualFrequencyResponseTest(FilterPass::lowPass, cutoffFrequency, 48000,
                                200);
    actualFrequencyResponseTest(FilterPass::highPass, cutoffFrequency, 48000,
                                331);
  }

  // test various sampling rates
  for (int samplingRate = 10; samplingRate < 400000; samplingRate *= 2) {
    actualFrequencyResponseTest(FilterPass::lowPass, samplingRate / 4,
                                samplingRate, std::sqrt(samplingRate));
    actualFrequencyResponseTest(FilterPass::highPass, samplingRate / 4,
                                samplingRate, std::sqrt(samplingRate) + 1);
  }
}

void transitionLengthTest(int transitionLength, int samplingRate,
                          double attenuationDB) {
  const int optimalCoefficientsCount = FIRFilter::getOptimalCoefficientsCount(
      samplingRate, attenuationDB, transitionLength);

  // odd number of coefficients
  BOOST_TEST(optimalCoefficientsCount % 2 == 1);

  const int calculatedTransitionLenght = FIRFilter::getTransitionLength(
      samplingRate, attenuationDB, optimalCoefficientsCount);

  // ensure that both formulas give synced output
  BOOST_TEST(transitionLength == calculatedTransitionLenght);
}

BOOST_AUTO_TEST_CASE(tansition_length_test) {
  transitionLengthTest(100, 48000, 40);
  transitionLengthTest(10, 48000, 40);
  transitionLengthTest(500, 20000, 100);
  transitionLengthTest(45, 12345, 42);
}

BOOST_AUTO_TEST_SUITE_END()
