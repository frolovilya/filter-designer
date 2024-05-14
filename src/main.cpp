#include "FFT.hpp"
#include "fir/FIRFilter.hpp"
#include "iir/IIRFilter.hpp"
#include "iir/RCGrid.hpp"
#include "Sampling.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

const unordered_map<string, string> argumentsToMap(int argc, char *argv[]) {
  auto arguments = unordered_map<string, string>{};

  for (int i = 1; i + 1 < argc; i += 2) {
    arguments[argv[i]] = argv[i + 1];
  }

  return arguments;
}

string parseParameter(const string parameterName,
                      const unordered_map<string, string> &arguments) {
  auto pos = arguments.find("--" + parameterName);
  if (pos == arguments.end()) {
    throw invalid_argument("'--" + parameterName +
                           "' parameter must be specified");
  } else {
    return pos->second;
  }
}

int parseIntParameter(const string parameterName, const int minValue,
                      const unordered_map<string, string> &arguments) {
  string stringValue = parseParameter(parameterName, arguments);
  int parsedValue = 0;
  if ((parsedValue = stoi(stringValue)) < minValue) {
    throw invalid_argument("'--" + parameterName +
                           "' must be >= " + to_string(minValue));
  }

  return parsedValue;
}

int parseFrequency(const unordered_map<string, string> &arguments) {
  return parseIntParameter("frequency", 1, arguments);
}

int parseSamplingRate(const unordered_map<string, string> &arguments) {
  return parseIntParameter("samplingRate", 1, arguments);
}

void designFIRFilter(const int cutoffFrequencyHz, const int samplingRateHz,
                     const unordered_map<string, string> &arguments) {
  int filterSize = parseIntParameter("filterSize", 100, arguments);
  cout << "Filter Size " << filterSize << "\n";

  FIRFilter firFilter = FIRFilter(cutoffFrequencyHz, filterSize,
                                  BlackmanWindow(), samplingRateHz);

  cout << "FIR Coefficients\n";
  auto coefficients = firFilter.getFilterCoefficients();
  for (const double &c : coefficients) {
    cout << c << " ";
  }
  cout << "\n";

  auto frequencyResponse = firFilter.calculateResponseDB(1, 1000);
  cout << "Frequency Response\n";
  for (const double &v : frequencyResponse) {
    cout << v << " ";
  }
  cout << "\n";
}

void designIIRFilter(const int cutoffFrequencyHz, const int samplingRateHz) {
  auto rcGrid = RCGrid(cutoffFrequencyHz, samplingRateHz);
  auto iirFilter = IIRFilter(rcGrid);

  IIRFilterCoefficients coefficients = rcGrid.getIIRFilterCoefficients();
  cout << "IIR Coefficients A=" << coefficients.a << "; B=" << coefficients.b
       << "\n";

  auto frequencyResponse = iirFilter.calculateResponseDB(1, 1000);
  cout << "Frequency Response\n";
  for (const double &v : frequencyResponse) {
    cout << v << " ";
  }
  cout << "\n";
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

    const string filterType = parseParameter("filter", arguments);
    if (filterType == "fir") {
      designFIRFilter(cutoffFrequencyHz, samplingRateHz, arguments);

    } else if (filterType == "iir") {
      designIIRFilter(cutoffFrequencyHz, samplingRateHz);

    } else {
      throw invalid_argument("--filter must be either 'fir' or 'iir");
    }

  } catch (const invalid_argument &err) {
    cout << err.what() << "\n";
  }

  return 0;
}