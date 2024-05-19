#include "../../shared/fir/FIRFilter.hpp"
#include "../../shared/fir/BlackmanWindow.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(FIRFilter_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
    BOOST_REQUIRE_THROW(FIRFilter(FilterPass::lowPass, -1, 100, BlackmanWindow(), 1000),
                      invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(FilterPass::lowPass, 100, -1, BlackmanWindow(), 1000),
                      invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(FilterPass::lowPass, 100, 100, BlackmanWindow(), -1),
                      invalid_argument);
  BOOST_REQUIRE_NO_THROW(FIRFilter(FilterPass::lowPass, 100, 200, BlackmanWindow(), 48000));
}

void idealFrequencyResponseTest(int cutoffFrequency, int samplingRate,
                                int filterSize) {
  FIRFilter filter =
      FIRFilter(FilterPass::lowPass, cutoffFrequency, filterSize, BlackmanWindow(), samplingRate);
  auto response = filter.generateIdealFrequencyResponse();

  // ensure that ideal response is symmetric
  const double tolerance = 0.0000001;
  for (int i = 0, j = response.size() - 1; i < j; i++, j--) {
    BOOST_TEST(abs(response[i] - response[j]) < tolerance);
  }
}

BOOST_AUTO_TEST_CASE(ideal_frequency_response_test) {
  idealFrequencyResponseTest(20, 100, 20);
  idealFrequencyResponseTest(1500, 21000, 373);
  idealFrequencyResponseTest(2000, 48000, 500);
}

void testFrequencyResponse(int cutoffFrequency, int samplingRate,
                           int filterSize) {
  // cout << "FIR Response cutoff=" << cutoffFrequency
  //      << ", samplingRate=" << samplingRate
  //      << ", filterSize=" << filterSize << "\n";
  FIRFilter filter =
      FIRFilter(FilterPass::lowPass, cutoffFrequency, filterSize, BlackmanWindow(), samplingRate);

  auto coefficients = filter.getFilterCoefficients();
  BOOST_TEST(coefficients.size() == filterSize);

  auto filterResponse = filter.calculateResponseDB(1, samplingRate / 2);
  const int transitionPeriod = 500;
  const double attenuationDB = -10;
  // rought test that attenuation is -10dB at cutoff frequency + transition
  // period
  BOOST_TEST(filterResponse[cutoffFrequency + transitionPeriod] <
             -attenuationDB);
}

BOOST_AUTO_TEST_CASE(response_test) {
  testFrequencyResponse(200, 24000, 200);
  testFrequencyResponse(500, 48000, 200);
  testFrequencyResponse(500, 48000, 1000);
  testFrequencyResponse(1000, 48000, 500);
  testFrequencyResponse(5000, 48000, 500);
  testFrequencyResponse(10000, 48000, 500);
}

void transitionLengthTest(int transitionLength, int samplingRate, double attenuationDB) {
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
