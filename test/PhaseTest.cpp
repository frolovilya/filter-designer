#include "../shared/Phase.hpp"
#include <boost/test/unit_test.hpp>
#include <cmath>

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
  phaseUnwrapTest({0, -M_PI, -2 * M_PI, -3 * M_PI, -3.5 * M_PI},
                  {0, M_PI, 2 * M_PI, 3 * M_PI, 2.5 * M_PI});

  phaseUnwrapTest({0, -M_PI, M_PI, -M_PI},
                  {0, M_PI, M_PI, M_PI});

  phaseUnwrapTest({0, M_PI, 2 + M_PI, 3 + M_PI},
                  {0, M_PI, 2 + M_PI, 3 + M_PI});
}

BOOST_AUTO_TEST_SUITE_END()
