#include "Backend.hpp"
#include "../shared/Sampling.hpp"
#include "../shared/fir/BlackmanWindow.hpp"
#include "../shared/fir/FIRFilter.hpp"
#include "../shared/fir/RectangularWindow.hpp"
#include "../shared/iir/IIRFilter.hpp"
#include "ListSelectorValues.hpp"
#include <QAreaSeries>
#include <QDebug>
#include <QQuickItem>
#include <QQuickView>
#include <QRandomGenerator>
#include <QXYSeries>
#include <QtMath>
#include <sstream>

Backend::Backend(QObject *parent)
    : QObject{parent}, coefficients{{}}, frequencyResponse{{}} {
  QObject::connect(this, &Backend::recalculationNeeded,
                   &Backend::recalculateCoefficientsAndFrequencyResponse);
}

int Backend::getSamplingRate() const { return samplingRate; }
int Backend::getSamplingRateRangeFrom() const {
  return defaultSamplingRateRange.from;
}
int Backend::getSamplingRateRangeTo() const {
  return defaultSamplingRateRange.to;
}
void Backend::setSamplingRate(int value) {
  if (value == samplingRate) {
    return;
  }
  samplingRate = std::max(std::min(value, getSamplingRateRangeTo()),
                          getSamplingRateRangeFrom());

  setVisibleFrequencyTo(nyquistFrequency(samplingRate));

  emit controlsStateChanged();
  emit recalculationNeeded();
}

int Backend::getCutoffFrequency() const { return cutoffFrequency; }
int Backend::getCutoffFrequencyRangeFrom() const {
  return defaultCutoffFrequencyRange.from;
}
int Backend::getCutoffFrequencyRangeTo() const {
  return std::min(defaultCutoffFrequencyRange.to,
                  nyquistFrequency(samplingRate));
}
void Backend::setCutoffFrequency(int value) {
  if (value == cutoffFrequency) {
    return;
  }
  cutoffFrequency = std::max(std::min(value, getCutoffFrequencyRangeTo()),
                             getCutoffFrequencyRangeFrom());

  if (cutoffFrequency > visibleFrequencyTo) {
    setVisibleFrequencyTo(cutoffFrequency *
                          displayedFrequencyResponseCutoffMult);
  }

  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getPassType() const {
  return QString::fromStdString(toString(passType));
}
QList<QString> Backend::getPassTypes() const {
    QList<QString> values;
    for (unsigned int i = 0; i < sizeof(passTypes) / sizeof(passTypes[0]); i++) {
        values.push_back(QString::fromStdString(passTypes[i].str));
    }

    return values;
}
void Backend::setPassType(QString value) {
  if (value.toStdString() == toString(passType)) {
    return;
  }
  passType = toPassType(value.toStdString());

  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getFilterType() const {
  return QString::fromStdString(toString(filterType));
}
QList<QString> Backend::getFilterTypes() const {
    QList<QString> values;
    for (unsigned int i = 0; i < sizeof(filterTypes) / sizeof(filterTypes[0]); i++) {
        values.push_back(QString::fromStdString(filterTypes[i].str));
    }

    return values;
}
void Backend::setFilterType(QString value) {
  if (value.toStdString() == toString(filterType)) {
    return;
  }
  filterType = toFilterType(value.toStdString());

  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getWindowType() const {
  return QString::fromStdString(toString(windowType));
}
QList<QString> Backend::getWindowTypes() const {
    QList<QString> values;
    for (unsigned int i = 0; i < sizeof(windowTypes) / sizeof(windowTypes[0]); i++) {
        values.push_back(QString::fromStdString(windowTypes[i].str));
    }

    return values;
}
void Backend::setWindowType(QString value) {
  if (value.toStdString() == toString(windowType)) {
    return;
  }
  windowType = toWindowType(value.toStdString());

  emit controlsStateChanged();
  emit recalculationNeeded();
}

int Backend::getAttenuationDB() const { return attenuationDB; }
int Backend::getAttenuationDBRangeFrom() const {
  return defaultAttenuationDBRange.from;
}
int Backend::getAttenuationDBRangeTo() const {
  return defaultAttenuationDBRange.to;
}
void Backend::setAttenuationDB(int value) {
  if (value == attenuationDB) {
    return;
  }
  attenuationDB = std::max(std::min(value, getAttenuationDBRangeTo()),
                           getAttenuationDBRangeFrom());

  if (useOptimalFilterSize) {
    setFilterSize(FIRFilter::getOptimalCoefficientsCount(
        samplingRate, attenuationDB, transitionLength));
  } else {
    setTransitionLength(FIRFilter::getTransitionLength(
        samplingRate, attenuationDB, filterSize));
  }

  emit controlsStateChanged();
}

int Backend::getTransitionLength() const { return transitionLength; }
int Backend::getTransitionLengthRangeFrom() const {
  return defaultTransitionLengthRange.from;
}
int Backend::getTransitionLengthRangeTo() const {
  return defaultTransitionLengthRange.to;
}
void Backend::setTransitionLength(int value) {
  if (value == transitionLength) {
    return;
  }
  transitionLength = std::max(std::min(value, getTransitionLengthRangeTo()),
                              getTransitionLengthRangeFrom());

  if (useOptimalFilterSize) {
    setFilterSize(FIRFilter::getOptimalCoefficientsCount(
        samplingRate, attenuationDB, transitionLength));
  }

  emit controlsStateChanged();
}

int Backend::getFilterSize() const { return filterSize; }
int Backend::getFilterSizeRangeFrom() const {
  return defaultFilterSizeRange.from;
}
int Backend::getFilterSizeRangeTo() const { return defaultFilterSizeRange.to; }
void Backend::setFilterSize(int value) {
  if (value == filterSize) {
    return;
  }
  filterSize = std::max(std::min(value, getFilterSizeRangeTo()),
                        getFilterSizeRangeFrom());

  if (!useOptimalFilterSize) {
    setTransitionLength(FIRFilter::getTransitionLength(
        samplingRate, attenuationDB, filterSize));
  }

  emit controlsStateChanged();
  emit recalculationNeeded();
}
bool Backend::isUseOptimalFilterSize() const { return useOptimalFilterSize; }
void Backend::setUseOptimalFilterSize(bool value) {
  if (value == useOptimalFilterSize) {
    return;
  }
  useOptimalFilterSize = value;

  emit controlsStateChanged();
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

double Backend::getFrequencyResponseMinValue() const {
  return *std::min_element(frequencyResponse.begin() + visibleFrequencyFrom - 1,
                           frequencyResponse.begin() + visibleFrequencyTo - 1);
}
double Backend::getFrequencyResponseMaxValue() const {
  return *std::max_element(frequencyResponse.begin() + visibleFrequencyFrom - 1,
                           frequencyResponse.begin() + visibleFrequencyTo - 1);
}

int Backend::getVisibleFrequencyFrom() const { return visibleFrequencyFrom; }
void Backend::setVisibleFrequencyFrom(int value) {
  if (value == visibleFrequencyFrom) {
    return;
  }
  visibleFrequencyFrom =
      std::min(std::max(value, getSamplingRateRangeFrom() / 2),
               getVisibleFrequencyTo() - 1);

  emit controlsStateChanged();
}
int Backend::getVisibleFrequencyTo() const { return visibleFrequencyTo; }
void Backend::setVisibleFrequencyTo(int value) {
  if (value == visibleFrequencyTo) {
    return;
  }
  visibleFrequencyTo = std::max(std::min(value, nyquistFrequency(samplingRate)),
                                getVisibleFrequencyFrom() + 1);

  emit controlsStateChanged();
}

/**
 * Recalculate coefficients and filter frequency response
 */
void Backend::recalculateCoefficientsAndFrequencyResponse() {
  std::unique_ptr<Filter> filter;

  if (filterType == FilterType::fir) {
    std::unique_ptr<Window> window;
    if (windowType == WindowType::blackman) {
      window = std::unique_ptr<Window>(new BlackmanWindow());
    } else {
      window = std::unique_ptr<Window>(new RectangularWindow());
    }

    qInfo() << "FIR cutoffFrequency=" << cutoffFrequency
            << "; filterSize=" << filterSize << "; window=" << toString(windowType)
            << "; samplingRate=" << samplingRate << "\n";

    filter = std::unique_ptr<Filter>(
        new FIRFilter(cutoffFrequency, filterSize, *window, samplingRate));
  } else {
    RCGrid rcGrid = RCGrid(cutoffFrequency, samplingRate);
    filter = std::unique_ptr<Filter>(new IIRFilter(rcGrid));
  }

  coefficients = filter->getFilterCoefficients();

  frequencyResponse =
      filter->calculateResponseDB(1, nyquistFrequency(samplingRate));

  emit calculationCompleted();
}

/**
 * Dynamically update QML LineSeries with new points
 */
void Backend::updateListSeries(QAbstractSeries *series,
                               const std::vector<double> &data, int from,
                               int to) {
  if (series) {
    int dataSize = std::min(static_cast<int>(data.size()), to) - from;
    QList<QPointF> points;
    points.reserve(dataSize);

    for (int j = from; j < dataSize + from; j++) {
      qreal x = j;
      qreal y = data[j];
      points.append(QPointF(x, y));
    }

    auto xySeries = static_cast<QXYSeries *>(series);
    // Use replace instead of clear + append, it's optimized for performance
    xySeries->replace(points);
  }
}

void Backend::updateCoefficients(QAbstractSeries *series) {
  updateListSeries(series, coefficients, visibleFrequencyFrom - 1,
                   visibleFrequencyTo - 1);
}

void Backend::updateFrequencyResponse(QAbstractSeries *series) {
  updateListSeries(series, frequencyResponse, visibleFrequencyFrom - 1,
                   visibleFrequencyTo - 1);
}
