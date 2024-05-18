#ifndef DEFAULTVALUES_HPP
#define DEFAULTVALUES_HPP

#include "valueRange.hpp"
#include "listSelectorValues.hpp"

constexpr int defaultSamplingRate = 48000;
constexpr ValueRange defaultSamplingRateRange{2, 200000};

constexpr int defaultCutoffFrequency = 200;
constexpr ValueRange defaultCutoffFrequencyRange{1, 40000};

constexpr int defaultAttenuationDB = 25;
constexpr ValueRange defaultAttenuationDBRange{1, 99};

constexpr int defaultTransitionLength = 100;
constexpr ValueRange defaultTransitionLengthRange{10, 1000};

constexpr int defaultFilterSize = 200;
constexpr ValueRange defaultFilterSizeRange{2, 10000};
constexpr bool defaultUseOptimalFilterSize = true;

constexpr PassType defaultPassType = PassType::lowPass;

constexpr FilterType defaultFilterType = FilterType::fir;

constexpr WindowType defaultWindowType = WindowType::blackman;

constexpr ValueRange defaultVisibleFrequencyRange{1, defaultSamplingRate / 2};
constexpr int minVisibleFrequencyResponseTo = 1000;
constexpr int displayedFrequencyResponseCutoffMult = 4;

#endif // DEFAULTVALUES_HPP
