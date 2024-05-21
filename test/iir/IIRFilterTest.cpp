#include "../../shared/iir/IIRFilter.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>

using namespace std;

BOOST_AUTO_TEST_SUITE(IIRFilter_test)

void testFrequencyResponse(int cutoffFrequency, int samplingRate) {
  cout << "IIR Response"
       << ", cutoff=" << cutoffFrequency << ", samplingRate=" << samplingRate
       << "\n";

  RCGrid rcGrid = RCGrid(cutoffFrequency, samplingRate);
  IIRFilter filter = IIRFilter(rcGrid);

  auto filterResponse = filter.calculateResponseDB();
  // for IIR expecting magnitude at cutoffFrequency at least < 0
  BOOST_TEST(filterResponse[cutoffFrequency] < 0);

  // ensure there're no NaN or Inf in response
  for (const double &magnitude : filterResponse) {
    BOOST_TEST(isfinite(magnitude));
  }
}

BOOST_AUTO_TEST_CASE(response_test) {
  testFrequencyResponse(100, 1000);
  testFrequencyResponse(2000, 10000);
  testFrequencyResponse(5000, 20000);
  testFrequencyResponse(10000, 40000);
  testFrequencyResponse(20000, 100000);
}

BOOST_AUTO_TEST_SUITE_END()
