#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QList>
#include <QPointF>
#include <string>
#include <vector>

QT_FORWARD_DECLARE_CLASS(QAbstractSeries)

class Backend : public QObject
{
    Q_OBJECT

public:
    explicit Backend(QObject *parent = nullptr);

    Q_PROPERTY(int samplingRateFrom MEMBER samplingRateFrom CONSTANT)
    const int samplingRateFrom = 2;
    Q_PROPERTY(int samplingRateTo MEMBER samplingRateTo CONSTANT)
    const int samplingRateTo = 200000;
    Q_PROPERTY(int defaultSamplingRate MEMBER defaultSamplingRate CONSTANT)
    const int defaultSamplingRate = 48000;

    Q_PROPERTY(int cutoffFrequencyFrom MEMBER cutoffFrequencyFrom CONSTANT)
    const int cutoffFrequencyFrom = 1;
    Q_PROPERTY(int cutoffFrequencyTo MEMBER cutoffFrequencyTo CONSTANT)
    const int cutoffFrequencyTo = 40000;
    Q_PROPERTY(int defaultCutoffFrequency MEMBER defaultCutoffFrequency CONSTANT)
    const int defaultCutoffFrequency = 200;

    Q_PROPERTY(int filterSizeFrom MEMBER filterSizeFrom CONSTANT)
    const int filterSizeFrom = 2;
    Q_PROPERTY(int filterSizeTo MEMBER filterSizeTo CONSTANT)
    const int filterSizeTo = 20000;
    Q_PROPERTY(int defaultFilterSize MEMBER defaultFilterSize CONSTANT)
    const int defaultFilterSize = 200;

    Q_INVOKABLE int getSamplingRate() const;
    Q_INVOKABLE int getCutoffFrequency() const;
    Q_INVOKABLE QString getPassType() const;
    Q_INVOKABLE QString getFilterType() const;
    Q_INVOKABLE QString getWindowType() const;
    Q_INVOKABLE int getFilterSize() const;

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
    void setFilterSize(int value);
    void recalculate();
    void updateCoefficients(QAbstractSeries *series);
    void updateFrequencyResponse(QAbstractSeries *series);

signals:
    void recalculationNeeded();
    void calculationCompleted();

private:
    int samplingRate;
    int cutoffFrequency;
    std::string passType;
    std::string filterType;
    std::string windowType;
    int filterSize;
    std::vector<double> coefficients;
    std::vector<double> frequencyResponse;
};

#endif // BACKEND_H
