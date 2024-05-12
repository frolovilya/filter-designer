#include "IIRFilter.hpp"
#include "RCGrid.hpp"
#include "SineWave.hpp"
#include <iostream>
#include <unordered_map>

using namespace std;

const unordered_map<string, string> argumentsToMap(int argc, char *argv[]) {
  auto arguments = unordered_map<string, string>{};

  for (int i = 1; i + 1 < argc; i += 2) {
    arguments[argv[i]] = argv[i + 1];
  }

  return arguments;
}

int parseIntParameter(const string parameterName, const int minValue,
                      const unordered_map<string, string> &arguments) {
  int parsedValue = 0;
  auto pos = arguments.find("--" + parameterName);
  if (pos == arguments.end()) {
    throw invalid_argument("'--" + parameterName +
                           "' parameter must be specified");
  } else {
    string value = pos->second;
    if ((parsedValue = stoi(value)) < minValue) {
      throw invalid_argument("'--" + parameterName +
                             "' must be >= " + to_string(minValue));
    }
  }

  return parsedValue;
}

int parseFrequency(const unordered_map<string, string> &arguments) {
  return parseIntParameter("frequency", 1, arguments);
}

int parseSamplingRate(const unordered_map<string, string> &arguments) {
  return parseIntParameter("samplingRate", 1, arguments);
}

double findMaxValue(const vector<double> &samples) {
  double max = 0;
  for (const double &sample : samples) {
    if (sample > max) {
      max = sample;
    }
  }

  return max;
}

int main(int argc, char *argv[]) {
  const auto arguments{argumentsToMap(argc, argv)};

  if (arguments.size() == 0) {
    cout << "Usage: filter-designer --frequency 100 --samplingRate 48000\n";
    return 0;
  }

  try {
    const int cutoffFrequencyHz = parseFrequency(arguments);
    cout << "Cutoff Frequency " << cutoffFrequencyHz << "Hz\n";

    const int samplingRateHz = parseSamplingRate(arguments);
    cout << "Sampling Rate " << samplingRateHz << "Hz\n";

    auto rcGrid = RCGrid(cutoffFrequencyHz, samplingRateHz);
    auto iirFilter = IIRFilter(rcGrid);

    IIRFilterCoefficients coefficients = rcGrid.getIIRFilterCoefficients();
    cout << "IIR Coefficients A=" << coefficients.a << "; B=" << coefficients.b
         << "\n";

    auto sine = SineWave(samplingRateHz);
    for (int frequency = 1; frequency < 1000; frequency++) {
      auto samples = sine.generatePeriod(frequency, 1);
      auto filteredSamples = iirFilter.apply(samples);

      cout << findMaxValue(filteredSamples) << " ";
    }
    cout << "\n";

  } catch (const invalid_argument &err) {
    cout << err.what() << "\n";
  }

  return 0;
}