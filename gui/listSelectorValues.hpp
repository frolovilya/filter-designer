#ifndef LISTSELECTORVALUES_H
#define LISTSELECTORVALUES_H

#include <string>

enum class WindowType { blackman, rectangular };

const struct {
  WindowType val;
  std::string str;
} windowTypes[] = {{WindowType::blackman, "Blackman"},
                   {WindowType::rectangular, "Rectangular"}};

std::string toString(WindowType t);
WindowType toWindowType(std::string str);

enum class FilterType { fir, iir };

const struct {
  FilterType val;
  std::string str;
} filterTypes[] = {{FilterType::fir, "FIR"}, {FilterType::iir, "IIR"}};

std::string toString(FilterType t);
FilterType toFilterType(std::string str);

enum class PassType { lowPass, highPass };

const struct {
  PassType val;
  std::string str;
} passTypes[] = {{PassType::lowPass, "Low Pass"},
                 {PassType::highPass, "High Pass"}};

std::string toString(PassType t);
PassType toPassType(std::string str);

#endif // LISTSELECTORVALUES_H
