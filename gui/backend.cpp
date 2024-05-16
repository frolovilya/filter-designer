#include "backend.hpp"
#include "../shared/fir/FIRFilter.hpp"
#include "../shared/fir/BlackmanWindow.hpp"
#include "../shared/fir/RectangularWindow.hpp"
#include "../shared/iir/IIRFilter.hpp"
#include <QDebug>
#include <sstream>

Backend::Backend(QObject *parent)
    : QObject{parent},
    samplingRate{defaultSamplingRate},
    cutoffFrequency{defaultCutoffFrequency},
    filterSize{defaultFilterSize},
    coefficients{{}}
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
    copy(coefficients.begin(), coefficients.end(), std::ostream_iterator<double>(ss, ", "));
    std::string s = ss.str();
    if (s.length() > 1) {
        s.pop_back();
        s.pop_back();
    }
    return QString::fromStdString(s);
}

void Backend::recalculate() {
    qInfo() << "Re-calculating coefficients\n";

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

    coefficients = (*filter).getFilterCoefficients();

    emit calculationCompleted();
}
