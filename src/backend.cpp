#include "backend.hpp"
#include <QDebug>
#include <sstream>

Backend::Backend(QObject *parent)
    : QObject{parent}, samplingRate{48000}, cutoffFrequency{200}, filterSize{200}, coefficients{{1, 2, 3}}
{
    QObject::connect(this, &Backend::recalculationNeeded, &Backend::recalculate);
}

int Backend::getSamplingRate() const {
    return samplingRate;
}
void Backend::setSamplingRate(int value) {
    samplingRate = value;
    emit recalculationNeeded();
}

int Backend::getCutoffFrequency() const {
    return cutoffFrequency;
}
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

int Backend::getFilterSize() const {
    return filterSize;
}
void Backend::setFilterSize(int value) {
    filterSize = value;
    emit recalculationNeeded();
}

QString Backend::getCoefficientsString() const {
    std::stringstream ss;
    copy(coefficients.begin(), coefficients.end(), std::ostream_iterator<int>(ss, ","));
    std::string s = ss.str();
    if (s.length() > 0) {
        s.pop_back();
    }
    return QString::fromStdString(s);
}

void Backend::recalculate() {
    qInfo() << "Re-calculating coefficients\n";
    emit calculationCompleted();
}
