#ifndef LISTSELECTORVALUES_H
#define LISTSELECTORVALUES_H

#include <string>
#include "../shared/FilterPass.hpp"

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

const struct {
  FilterPass val;
  std::string str;
} passTypes[] = {{FilterPass::lowPass, "Low Pass"},
                 {FilterPass::highPass, "High Pass"}};

std::string toString(FilterPass t);
FilterPass toPassType(std::string str);

#endif // LISTSELECTORVALUES_H
