#ifndef BACKEND_H
#define BACKEND_H

#include <QList>
#include <QObject>
#include <QPointF>
#include <string>
#include <vector>

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
  Q_INVOKABLE QString getFilterType() const;
  Q_INVOKABLE QString getWindowType() const;

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

  Q_INVOKABLE int getFrequencyResponseBinsCount() const;
  Q_INVOKABLE double getFrequencyResponseMinValue() const;
  Q_INVOKABLE double getFrequencyResponseMaxValue() const;

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
  void recalculate();
  void updateCoefficients(QAbstractSeries *series);
  void updateFrequencyResponse(QAbstractSeries *series);

signals:
  void controlsStateChanged();
  void recalculationNeeded();
  void calculationCompleted();

private:
  const int defaultSamplingRate = 48000;
  const int defaultSamplingRateRangeFrom = 2;
  const int defaultSamplingRateRangeTo = 200000;
  const int defaultCutoffFrequency = 200;
  const int defaultCutoffFrequencyRangeFrom = 1;
  const int defaultCutoffFrequencyRangeTo = 40000;
  const int defaultAttenuationDB = 25;
  const int defaultAttenuationDBRangeFrom = 1;
  const int defaultAttenuationDBRangeTo = 99;
  const int defaultTransitionLength = 100;
  const int defaultTransitionLengthRangeFrom = 10;
  const int defaultTransitionLengthRangeTo = 1000;
  const int defaultFilterSize = 200;
  const int defaultFilterSizeRangeFrom = 2;
  const int defaultFilterSizeRangeTo = 10000;

  const int minDisplayedFrequencyResponseRange = 1000;
  const int displayedFrequencyResponseCutoffMult = 4;

  int samplingRate;
  int cutoffFrequency;
  std::string passType;
  std::string filterType;
  std::string windowType;
  int attenuationDB;
  int transitionLength;
  int filterSize;
  bool useOptimalFilterSize;
  std::vector<double> coefficients;
  std::vector<double> frequencyResponse;

  void updateListSeries(QAbstractSeries *series, const std::vector<double>& data);
};

#endif // BACKEND_H
