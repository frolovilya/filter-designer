#include "../src/IIRFilter.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(IIRFilter_test)

BOOST_AUTO_TEST_CASE(response_test) {
  const int cutoffFrequencyHz = 500;
  const int samplingRateHz = 48000;
  RCGrid rcGrid = RCGrid(cutoffFrequencyHz, samplingRateHz);
  IIRFilter filter = IIRFilter(rcGrid);

  auto filterResponse = filter.calculateResponseDB(1, 1000);
  // expecting magnitude at cutoff frequency to be roughly -3dB +- 1dB
  BOOST_TEST(abs(filterResponse[cutoffFrequencyHz] + 3) < 1);
}

BOOST_AUTO_TEST_SUITE_END()