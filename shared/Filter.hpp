#ifndef FILTER_H
#define FILTER_H

#include <vector>

class Filter {
public:
  virtual ~Filter() {}

  virtual int getCutoffFrequency() const = 0;
  virtual int getSamplingRate() const = 0;
  virtual std::vector<double> getFilterCoefficients() const = 0;
  virtual std::vector<double> calculateResponseDB() const = 0;
};

#endif
