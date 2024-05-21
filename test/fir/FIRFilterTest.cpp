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
  BOOST_REQUIRE_NO_THROW(
      FIRFilter(FilterPass::lowPass, 100, 200, BlackmanWindow(), 48000));
}

void idealFrequencyResponseTest(FilterPass pass, int cutoffFrequency,
                                int samplingRate, int filterSize) {
  FIRFilter filter = FIRFilter(pass, cutoffFrequency, filterSize,
                               BlackmanWindow(), samplingRate);
  auto response = filter.generateIdealFrequencyResponse();

  // ensure that ideal response is symmetric
  const double tolerance = 0.0000001;
  for (int i = 0, j = response.size() - 1; i < j; i++, j--) {
    BOOST_TEST(abs(response[i] - response[j]) < tolerance);
  }
}

BOOST_AUTO_TEST_CASE(ideal_frequency_response_test) {
  idealFrequencyResponseTest(FilterPass::lowPass, 20, 100, 20);
  idealFrequencyResponseTest(FilterPass::lowPass, 1500, 21000, 373);
  idealFrequencyResponseTest(FilterPass::lowPass, 2000, 48000, 500);
  idealFrequencyResponseTest(FilterPass::highPass, 10000, 48000, 500);
  idealFrequencyResponseTest(FilterPass::highPass, 20000, 48000, 333);
  idealFrequencyResponseTest(FilterPass::highPass, 7000, 15000, 400);
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

  // ensure there're no Inf or NaN coefficients
  for (const double &c : coefficients) {
    // abort test to avoid spamming with Inf test errors
    BOOST_TEST_REQUIRE(std::isfinite(c));
  }

  auto filterResponse = filter.calculateResponseDB();
  const int transitionPeriod = 500;
  const double attenuationDB = -10;
  // rought test that attenuation is -10dB at cutoff frequency + transition
  // period
  BOOST_TEST(filterResponse[cutoffFrequency + transitionPeriod] <
             -attenuationDB);
}

BOOST_AUTO_TEST_CASE(response_test) {
  actualFrequencyResponseTest(FilterPass::lowPass, 200, 24000, 200);
  actualFrequencyResponseTest(FilterPass::lowPass, 500, 48000, 200);
  actualFrequencyResponseTest(FilterPass::lowPass, 500, 48000, 223);
  actualFrequencyResponseTest(FilterPass::lowPass, 500, 48000, 1000);
  actualFrequencyResponseTest(FilterPass::lowPass, 1000, 48000, 500);
  actualFrequencyResponseTest(FilterPass::lowPass, 5000, 48000, 500);
  actualFrequencyResponseTest(FilterPass::lowPass, 10000, 48000, 500);
  actualFrequencyResponseTest(FilterPass::lowPass, 1000, 150000, 300);
  actualFrequencyResponseTest(FilterPass::lowPass, 60000, 150000, 300);

  actualFrequencyResponseTest(FilterPass::highPass, 74000, 150000, 440);
  actualFrequencyResponseTest(FilterPass::highPass, 2000, 150000, 600);
  actualFrequencyResponseTest(FilterPass::highPass, 23500, 48000, 200);
  actualFrequencyResponseTest(FilterPass::highPass, 20000, 48000, 100);
  actualFrequencyResponseTest(FilterPass::highPass, 15000, 48000, 555);
  actualFrequencyResponseTest(FilterPass::highPass, 1000, 48000, 1000);
  actualFrequencyResponseTest(FilterPass::highPass, 500, 24000, 300);
}

void transitionLengthTest(int transitionLength, int samplingRate,
                          double attenuationDB) {
  const int optimalCoefficientsCount = FIRFilter::getOptimalCoefficientsCount(
      samplingRate, attenuationDB, transitionLength);

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
