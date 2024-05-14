#include "../../src/iir/RCGrid.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(RCGrid_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(RCGrid(-1, 100), invalid_argument);
  BOOST_REQUIRE_THROW(RCGrid(100, -1), invalid_argument);
  BOOST_REQUIRE_THROW(RCGrid(100, -1), invalid_argument);
  BOOST_REQUIRE_NO_THROW(RCGrid(100, 200));
}

BOOST_AUTO_TEST_CASE(coefficients_test) {
  const int cutoffFrequencyHz = 100;
  const int samplingRateHz = 1000;

  auto rcGrid = RCGrid(cutoffFrequencyHz, samplingRateHz);
  auto coefficients = rcGrid.getIIRFilterCoefficients();

  BOOST_TEST(coefficients.a > 0);
  BOOST_TEST(coefficients.b > 0);
}

BOOST_AUTO_TEST_SUITE_END()