#include "backend.hpp"
#include "../shared/Sampling.hpp"
#include "../shared/fir/BlackmanWindow.hpp"
#include "../shared/fir/FIRFilter.hpp"
#include "../shared/fir/RectangularWindow.hpp"
#include "../shared/iir/IIRFilter.hpp"
#include <QAreaSeries>
#include <QDebug>
#include <QQuickItem>
#include <QQuickView>
#include <QRandomGenerator>
#include <QXYSeries>
#include <QtMath>
#include <sstream>

Backend::Backend(QObject *parent)
    : QObject{parent}, samplingRate{defaultSamplingRate},
      cutoffFrequency{defaultCutoffFrequency}, filterSize{defaultFilterSize},
      coefficients{{}} {
  QObject::connect(this, &Backend::recalculationNeeded, &Backend::recalculate);
}

int Backend::getSamplingRate() const { return samplingRate; }
void Backend::setSamplingRate(int value) {
  samplingRate = value;
  emit recalculationNeeded();
}

int Backend::getCutoffFrequency() const { return cutoffFrequency; }
void Backend::setCutoffFrequency(int value) {
  cutoffFrequency = value;
  emit recalculationNeeded();
}

QString Backend::getPassType() const {
  return QString::fromStdString(passType);
}
void Backend::setPassType(QString value) {
  passType = value.toStdString();
  emit recalculationNeeded();
}

QString Backend::getFilterType() const {
  return QString::fromStdString(filterType);
}
void Backend::setFilterType(QString value) {
  filterType = value.toStdString();
  emit recalculationNeeded();
}

QString Backend::getWindowType() const {
  return QString::fromStdString(windowType);
}
void Backend::setWindowType(QString value) {
  windowType = value.toStdString();
  emit recalculationNeeded();
}

int Backend::getFilterSize() const { return filterSize; }
void Backend::setFilterSize(int value) {
  filterSize = value;
  emit recalculationNeeded();
}

QString Backend::getCoefficientsString() const {
  std::stringstream ss;
  copy(coefficients.begin(), coefficients.end(),
       std::ostream_iterator<double>(ss, ", "));
  std::string s = ss.str();
  if (s.length() > 1) {
    s.pop_back();
    s.pop_back();
  }
  return QString::fromStdString(s);
}

int Backend::getCoefficientsCount() const { return coefficients.size(); }
double Backend::getCoefficientsMinValue() const {
  return *std::min_element(coefficients.begin(), coefficients.end());
}
double Backend::getCoefficientsMaxValue() const {
  return *std::max_element(coefficients.begin(), coefficients.end());
}

int Backend::getFrequencyResponseBinsCount() const {
  return frequencyResponse.size();
}
double Backend::getFrequencyResponseMinValue() const {
  return *std::min_element(frequencyResponse.begin(), frequencyResponse.end());
}
double Backend::getFrequencyResponseMaxValue() const {
  return *std::max_element(frequencyResponse.begin(), frequencyResponse.end());
}

void Backend::recalculate() {
  std::unique_ptr<Filter> filter;

  if (filterType == "FIR") {
    std::unique_ptr<Window> window;
    if (windowType == "Blackman") {
      window = std::unique_ptr<Window>(new BlackmanWindow());
    } else {
      window = std::unique_ptr<Window>(new RectangularWindow());
    }

    filter = std::unique_ptr<Filter>(
        new FIRFilter(cutoffFrequency, filterSize, *window, samplingRate));
  } else {
    RCGrid rcGrid = RCGrid(cutoffFrequency, samplingRate);
    filter = std::unique_ptr<Filter>(new IIRFilter(rcGrid));
  }

  coefficients = filter->getFilterCoefficients();

  const int minDisplayedFrequencyResponseRange = 1000;
  const int displayedFrequencyResponseCutoffMult = 4;
  frequencyResponse = filter->calculateResponseDB(
      1,
      std::max(std::min(nyquistFrequency(samplingRate),
                        cutoffFrequency * displayedFrequencyResponseCutoffMult),
               minDisplayedFrequencyResponseRange));

  emit calculationCompleted();
}

void Backend::updateCoefficients(QAbstractSeries *series) {
  if (series) {
    QList<QPointF> points;
    points.reserve(coefficients.size());
    for (unsigned int j = 0; j < coefficients.size(); j++) {
      qreal x = j;
      qreal y = coefficients[j];
      points.append(QPointF(x, y));
    }

    auto xySeries = static_cast<QXYSeries *>(series);
    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);
  }
}

void Backend::updateFrequencyResponse(QAbstractSeries *series) {
  if (series) {
    QList<QPointF> points;
    points.reserve(frequencyResponse.size());
    for (unsigned int j = 0; j < frequencyResponse.size(); j++) {
      qreal x = j;
      qreal y = frequencyResponse[j];
      points.append(QPointF(x, y));
    }

    auto xySeries = static_cast<QXYSeries *>(series);
    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);
  }
}
