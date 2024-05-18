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
      cutoffFrequency{defaultCutoffFrequency},
      attenuationDB{defaultAttenuationDB},
      transitionLength{defaultTransitionLength}, filterSize{defaultFilterSize},
    useOptimalFilterSize{true}, coefficients{{}} {
  QObject::connect(this, &Backend::recalculationNeeded, &Backend::recalculate);
}

int Backend::getSamplingRate() const { return samplingRate; }
int Backend::getSamplingRateRangeFrom() const {
  return defaultSamplingRateRangeFrom;
}
int Backend::getSamplingRateRangeTo() const {
  return defaultSamplingRateRangeTo;
}
void Backend::setSamplingRate(int value) {
  samplingRate = value;
  emit controlsStateChanged();
  emit recalculationNeeded();
}

int Backend::getCutoffFrequency() const { return cutoffFrequency; }
int Backend::getCutoffFrequencyRangeFrom() const {
  return defaultCutoffFrequencyRangeFrom;
}
int Backend::getCutoffFrequencyRangeTo() const {
  return std::min(defaultCutoffFrequencyRangeTo, samplingRate / 2);
}
void Backend::setCutoffFrequency(int value) {
  cutoffFrequency = value;
  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getPassType() const {
  return QString::fromStdString(passType);
}
void Backend::setPassType(QString value) {
  passType = value.toStdString();
  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getFilterType() const {
  return QString::fromStdString(filterType);
}
void Backend::setFilterType(QString value) {
  filterType = value.toStdString();
  emit controlsStateChanged();
  emit recalculationNeeded();
}

QString Backend::getWindowType() const {
  return QString::fromStdString(windowType);
}
void Backend::setWindowType(QString value) {
  windowType = value.toStdString();
  emit controlsStateChanged();
  emit recalculationNeeded();
}

int Backend::getAttenuationDB() const { return attenuationDB; }
int Backend::getAttenuationDBRangeFrom() const {
  return defaultAttenuationDBRangeFrom;
}
int Backend::getAttenuationDBRangeTo() const {
  return defaultAttenuationDBRangeTo;
}
void Backend::setAttenuationDB(int value) {
  attenuationDB = value;

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
  return defaultTransitionLengthRangeFrom;
}
int Backend::getTransitionLengthRangeTo() const {
  return defaultTransitionLengthRangeTo;
}
void Backend::setTransitionLength(int value) {
  transitionLength = value;

  if (useOptimalFilterSize) {
    setFilterSize(FIRFilter::getOptimalCoefficientsCount(
        samplingRate, attenuationDB, transitionLength));
  }

  emit controlsStateChanged();
}

int Backend::getFilterSize() const { return filterSize; }
int Backend::getFilterSizeRangeFrom() const {
  return defaultFilterSizeRangeFrom;
}
int Backend::getFilterSizeRangeTo() const { return defaultFilterSizeRangeTo; }
void Backend::setFilterSize(int value) {
  filterSize = value;

  if (!useOptimalFilterSize) {
    setTransitionLength(FIRFilter::getTransitionLength(
        samplingRate, attenuationDB, filterSize));
  }

  emit controlsStateChanged();
  emit recalculationNeeded();
}
bool Backend::isUseOptimalFilterSize() const { return useOptimalFilterSize; }
void Backend::setUseOptimalFilterSize(bool value) {
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

int Backend::getFrequencyResponseBinsCount() const {
  return frequencyResponse.size();
}
double Backend::getFrequencyResponseMinValue() const {
  return *std::min_element(frequencyResponse.begin(), frequencyResponse.end());
}
double Backend::getFrequencyResponseMaxValue() const {
  return *std::max_element(frequencyResponse.begin(), frequencyResponse.end());
}

/**
 * Recalculate coefficients and filter frequency response
 */
void Backend::recalculate() {
  std::unique_ptr<Filter> filter;

  if (filterType == "FIR") {
    std::unique_ptr<Window> window;
    if (windowType == "Blackman") {
      window = std::unique_ptr<Window>(new BlackmanWindow());
    } else {
      window = std::unique_ptr<Window>(new RectangularWindow());
    }

    qInfo() << "FIR cutoffFrequency=" << cutoffFrequency
            << "; filterSize=" << filterSize << "; window=" << windowType
            << "; samplingRate=" << samplingRate << "\n";

    filter = std::unique_ptr<Filter>(
        new FIRFilter(cutoffFrequency, filterSize, *window, samplingRate));
  } else {
    RCGrid rcGrid = RCGrid(cutoffFrequency, samplingRate);
    filter = std::unique_ptr<Filter>(new IIRFilter(rcGrid));
  }

  coefficients = filter->getFilterCoefficients();

  frequencyResponse = filter->calculateResponseDB(
      1,
      std::max(std::min(nyquistFrequency(samplingRate),
                        cutoffFrequency * displayedFrequencyResponseCutoffMult),
               minDisplayedFrequencyResponseRange));

  /*for (const double &f : frequencyResponse) {
      qInfo() << f << " ";
  }
  qInfo() << "\n";*/

  emit calculationCompleted();
}

/**
 * Dynamically update QML LineSeries with new points
 */
void Backend::updateListSeries(QAbstractSeries *series,
                               const std::vector<double> &data) {
  if (series) {
    QList<QPointF> points;
    points.reserve(data.size());
    for (unsigned int j = 0; j < data.size(); j++) {
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
  updateListSeries(series, coefficients);
}

void Backend::updateFrequencyResponse(QAbstractSeries *series) {
  updateListSeries(series, frequencyResponse);
}
