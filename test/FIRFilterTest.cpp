#include "../src/FIRFilter.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(FIRFilter_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(FIRFilter(-1, 100, BlackmanWindow(), 1000), invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(100, -1, BlackmanWindow(), 1000), invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(100, 100, BlackmanWindow(), -1), invalid_argument);
  BOOST_REQUIRE_NO_THROW(FIRFilter(100, 200, BlackmanWindow(), 48000));
}

BOOST_AUTO_TEST_CASE(response_test) {
  const int cutoffFrequencyHz = 500;
  const int samplingRateHz = 48000;
  const int filterSize = 200;
  FIRFilter filter = FIRFilter(cutoffFrequencyHz, filterSize, BlackmanWindow(), samplingRateHz);

  auto coefficients = filter.getFilterCoefficients();
  BOOST_TEST(coefficients.size() == filterSize);

  auto filterResponse = filter.calculateResponseDB(1, 1000);
  // rought test that attenuation is -25dB at cutoff frequency
  BOOST_TEST(filterResponse[cutoffFrequencyHz] < -25);
}

BOOST_AUTO_TEST_SUITE_END()