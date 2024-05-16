#include "../../shared/fir/BlackmanWindow.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BlackmanWindow_test)

BOOST_AUTO_TEST_CASE(coefficients_test) {
  const int windowSize = 10;

  auto window = BlackmanWindow();
  auto coefficients = window.getCoefficients(windowSize);

  BOOST_TEST(coefficients.size() == windowSize);
}

BOOST_AUTO_TEST_CASE(window_apply_test) {
  std::vector<double> samples;
  for (int i = 0; i < 10; i++) {
    samples.push_back(i / 100);
  }

  auto window = BlackmanWindow();
  auto windowedSamples = window.apply(samples);

  BOOST_TEST(samples.size() == windowedSamples.size());
  BOOST_TEST(samples <= windowedSamples);
}

BOOST_AUTO_TEST_SUITE_END()
