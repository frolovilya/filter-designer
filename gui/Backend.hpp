#ifndef BACKEND_H
#define BACKEND_H

#include <QList>
#include <QObject>
#include <QPointF>
#include <vector>
#include "ListSelectorValues.hpp"
#include "ValueRange.hpp"
#include "DefaultControlValues.hpp"
#include "../shared/FilterResponse.hpp"

QT_FORWARD_DECLARE_CLASS(QAbstractSeries)

class Backend : public QObject {
  Q_OBJECT

public:
  explicit Backend(QObject *parent = nullptr);

  Q_INVOKABLE int getSamplingRate() const;
  Q_INVOKABLE int getSamplingRateRangeFrom() const;
  Q_INVOKABLE int getSamplingRateRangeTo() const;

  Q_INVOKABLE int getCutoffFrequency() const;
  Q_INVOKABLE int getCutoffFrequencyRangeFrom() const;
  Q_INVOKABLE int getCutoffFrequencyRangeTo() const;

  Q_INVOKABLE QString getPassType() const;
  Q_INVOKABLE QList<QString> getPassTypes() const;
  Q_INVOKABLE QString getFilterType() const;
  Q_INVOKABLE QList<QString> getFilterTypes() const;
  Q_INVOKABLE QString getWindowType() const;
  Q_INVOKABLE QList<QString> getWindowTypes() const;

  Q_INVOKABLE int getAttenuationDB() const;
  Q_INVOKABLE int getAttenuationDBRangeFrom() const;
  Q_INVOKABLE int getAttenuationDBRangeTo() const;

  Q_INVOKABLE int getTransitionLength() const;
  Q_INVOKABLE int getTransitionLengthRangeFrom() const;
  Q_INVOKABLE int getTransitionLengthRangeTo() const;

  Q_INVOKABLE int getFilterSize() const;
  Q_INVOKABLE int getFilterSizeRangeFrom() const;
  Q_INVOKABLE int getFilterSizeRangeTo() const;
  Q_INVOKABLE bool isUseOptimalFilterSize() const;

  Q_INVOKABLE int getCoefficientsCount() const;
  Q_INVOKABLE double getCoefficientsMinValue() const;
  Q_INVOKABLE double getCoefficientsMaxValue() const;
  Q_INVOKABLE QString getCoefficientsString() const;

  Q_INVOKABLE double getFrequencyResponseMinValue() const;
  Q_INVOKABLE double getFrequencyResponseMaxValue() const;

  Q_INVOKABLE double getPhaseResponseMaxValue() const;
  Q_INVOKABLE double getPhaseResponseMinValue() const;

  Q_INVOKABLE int getVisibleFrequencyFrom() const;
  Q_INVOKABLE int getVisibleFrequencyTo() const;

public slots:
  void setSamplingRate(int value);
  void setCutoffFrequency(int value);
  void setPassType(QString value);
  void setFilterType(QString value);
  void setWindowType(QString value);
  void setAttenuationDB(int value);
  void setTransitionLength(int value);
  void setFilterSize(int value);
  void setUseOptimalFilterSize(bool value);
  void setVisibleFrequencyFrom(int value);
  void setVisibleFrequencyTo(int value);
  void recalculateCoefficientsAndFrequencyResponse();
  void updateCoefficients(QAbstractSeries *series);
  void updateFrequencyResponse(QAbstractSeries *series);
  void updatePhaseShifts(QAbstractSeries *series);

signals:
  void controlsStateChanged();
  void recalculationNeeded();
  void calculationCompleted();

private:
  int samplingRate = defaultSamplingRate;
  int cutoffFrequency = defaultCutoffFrequency;
  FilterPass passType = defaultPassType;
  FilterType filterType = defaultFilterType;
  WindowType windowType = defaultWindowType;
  int attenuationDB = defaultAttenuationDB;
  int transitionLength = defaultTransitionLength;
  int filterSize = defaultFilterSize;
  bool useOptimalFilterSize = defaultUseOptimalFilterSize;
  int visibleFrequencyFrom = defaultVisibleFrequencyRange.from;
  int visibleFrequencyTo = defaultVisibleFrequencyRange.to;
  std::vector<double> coefficients;
  std::vector<FilterResponse> filterResponse;

  void updateListSeries(QAbstractSeries *series,
                        const std::vector<double> &data, int from, int to);
};

#endif // BACKEND_H
