#include "../../shared/fir/FIRFilter.hpp"
#include "../../shared/fir/BlackmanWindow.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(FIRFilter_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
  BOOST_REQUIRE_THROW(FIRFilter(-1, 100, BlackmanWindow(), 1000), invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(100, -1, BlackmanWindow(), 1000), invalid_argument);
  BOOST_REQUIRE_THROW(FIRFilter(100, 100, BlackmanWindow(), -1), invalid_argument);
  BOOST_REQUIRE_NO_THROW(FIRFilter(100, 200, BlackmanWindow(), 48000));
}

void testFrequencyResponse(int cutoffFrequency, int samplingRate, int filterSize) {
    //cout << "FIR Response cutoff=" << cutoffFrequency
    //     << ", samplingRate=" << samplingRate
    //     << ", filterSize=" << filterSize << "\n";
    FIRFilter filter = FIRFilter(cutoffFrequency, filterSize, BlackmanWindow(), samplingRate);

    auto coefficients = filter.getFilterCoefficients();
    BOOST_TEST(coefficients.size() == filterSize);

    auto filterResponse = filter.calculateResponseDB(1, samplingRate / 2);
    const int transitionPeriod = 500;
    // rought test that attenuation is -10dB at cutoff frequency + transition period
    BOOST_TEST(filterResponse[cutoffFrequency + transitionPeriod] < -10);
}

BOOST_AUTO_TEST_CASE(response_test) {
  testFrequencyResponse(200, 24000, 200);
  testFrequencyResponse(500, 48000, 200);
  testFrequencyResponse(500, 48000, 1000);
  testFrequencyResponse(1000, 48000, 500);
  testFrequencyResponse(5000, 48000, 500);
  testFrequencyResponse(10000, 48000, 500);
}

BOOST_AUTO_TEST_SUITE_END()
