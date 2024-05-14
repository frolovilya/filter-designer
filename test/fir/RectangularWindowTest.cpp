#include "../../src/fir/RectangularWindow.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(RectangularWindow_test)

BOOST_AUTO_TEST_CASE(coefficients_test) {
  const int windowSize = 10;

  auto window = RectangularWindow();
  auto coefficients = window.getCoefficients(windowSize);

  BOOST_TEST(coefficients.size() == windowSize);

  for (const double &coefficient : coefficients) {
    BOOST_TEST(coefficient == 1);
  }
}

BOOST_AUTO_TEST_CASE(window_apply_test) {
  std::vector<double> samples;
  for (int i = 0; i < 10; i++) {
    samples.push_back(i / 100);
  }

  auto window = RectangularWindow();
  auto windowedSamples = window.apply(samples);

  BOOST_TEST(samples == windowedSamples);
}

BOOST_AUTO_TEST_SUITE_END()