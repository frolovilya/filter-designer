#include "../shared/Phase.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <numbers>

using namespace std;

BOOST_AUTO_TEST_SUITE(Phase_test)

void phaseUnwrapTest(std::vector<double> inputAngles,
                     std::vector<double> expectedResult) {
  std::vector<double> actualResult = phaseUnwrap(inputAngles);
  BOOST_TEST(actualResult.size() == expectedResult.size());

  const double tolerance = 1e-7;
  for (unsigned int i = 0; i < actualResult.size(); i++) {
    BOOST_TEST(abs(expectedResult[i] - actualResult[i]) < tolerance);
  }
}

BOOST_AUTO_TEST_CASE(phase_unwrap_test) {
  // expecting to add 2 * Pi to angle diffs >= Pi
  phaseUnwrapTest({0, -numbers::pi, -2 * numbers::pi, -3 * numbers::pi, -3.5 * numbers::pi},
                  {0, numbers::pi, 2 * numbers::pi, 3 * numbers::pi, 2.5 * numbers::pi});

  phaseUnwrapTest({0, -numbers::pi, numbers::pi, -numbers::pi},
                  {0, numbers::pi, numbers::pi, numbers::pi});

  phaseUnwrapTest({0, numbers::pi, 2 + numbers::pi, 3 + numbers::pi},
                  {0, numbers::pi, 2 + numbers::pi, 3 + numbers::pi});
}

BOOST_AUTO_TEST_SUITE_END()
