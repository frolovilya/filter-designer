#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <string>
#include <vector>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

    Q_INVOKABLE int getSamplingRate() const;
    Q_INVOKABLE int getCutoffFrequency() const;
    Q_INVOKABLE QString getPassType() const;
    Q_INVOKABLE QString getFilterType() const;
    Q_INVOKABLE QString getWindowType() const;
    Q_INVOKABLE int getFilterSize() const;
    Q_INVOKABLE QString getCoefficientsString() const;

public slots:
    void setSamplingRate(int value);
    void setCutoffFrequency(int value);
    void setPassType(QString value);
    void setFilterType(QString value);
    void setWindowType(QString value);
    void setFilterSize(int value);
    void recalculate();

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
};

#endif // BACKEND_H
