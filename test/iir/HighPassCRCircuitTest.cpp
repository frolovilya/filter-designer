#include "../../shared/iir/HighPassCRCircuit.hpp"
#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(HighPassCRCircuit_test)

BOOST_AUTO_TEST_CASE(constructor_test) {
    BOOST_REQUIRE_THROW(HighPassCRCircuit(-1, 100), invalid_argument);
    BOOST_REQUIRE_THROW(HighPassCRCircuit(100, -1), invalid_argument);
    BOOST_REQUIRE_THROW(HighPassCRCircuit(100, -1), invalid_argument);
    BOOST_REQUIRE_THROW(HighPassCRCircuit(100, 200), invalid_argument);
    BOOST_REQUIRE_NO_THROW(HighPassCRCircuit(99, 200));
}

void testCoefficients(int cutoffFrequency, int samplingRate) {
    auto circuit = HighPassCRCircuit(cutoffFrequency, samplingRate);
    auto coefficients = circuit.getFilterCoefficients();

    BOOST_TEST(coefficients.size() == 3);
    BOOST_TEST(coefficients[0] > 0);
    BOOST_TEST(coefficients[1] < 0);
    BOOST_TEST(coefficients[2] > 0);
}

BOOST_AUTO_TEST_CASE(coefficients_test) {
    testCoefficients(100, 1000);
    testCoefficients(1000, 10000);
    testCoefficients(10000, 48000);
    testCoefficients(20000, 100000);
}

void testFrequencyResponse(int cutoffFrequency, int samplingRate) {
  cout << "IIR CR Circuit Response"
       << ", cutoff=" << cutoffFrequency << ", samplingRate=" << samplingRate
       << "\n";

  HighPassCRCircuit circuit = HighPassCRCircuit(cutoffFrequency, samplingRate);

  auto filterResponse = circuit.calculateResponse();
  // for IIR expecting magnitude at cutoffFrequency at least < 0
  BOOST_TEST(filterResponse[cutoffFrequency].magnitudeDB < 0);

  // simple is high pass check
  BOOST_TEST(abs(filterResponse[0].magnitudeDB) > abs(filterResponse[filterResponse.size() - 1].magnitudeDB));

  // test that response magnitudes are negative
  for (const FilterResponse &response : filterResponse) {
    BOOST_TEST(response.magnitudeDB <= 0);
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
