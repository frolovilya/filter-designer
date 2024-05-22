import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtCharts
import filter.designer.qmlcomponents

ChartView {
    id: frequencyResponse
    title: "Frequency Response"
    Layout.minimumWidth: 650
    Layout.minimumHeight: 370
    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.margins: -margin
    legend.visible: false
    //antialiasing: true
    backgroundRoundness: 0
    backgroundColor: "black"
    titleColor: "white"
    margins.left: margin
    margins.right: margin
    margins.top: 0
    margins.bottom: margin

    ValuesAxis {
        id: frequencyAxisX
        min: 1
        max: 1000
        gridLineColor: "dimgray"
        gridVisible: true
        lineVisible: false
        labelFormat: "%.0f"
        labelsColor: "dimgray"
        titleText: "Frequency (Hz)"
    }

    ValuesAxis {
        id: magnitudeAxisY
        min: -100
        max: 0
        gridLineColor: "dimgray"
        gridVisible: true
        lineVisible: false
        labelFormat: "%.0f"
        labelsColor: "dimgray"
        titleText: "Magnitude (dB)"
    }

    AreaSeries {
        id: passBand
        axisX: frequencyAxisX
        axisY: magnitudeAxisY
        color: "darkolivegreen"
        borderWidth: 0
        opacity: 0.8
    }

    AreaSeries {
        id: transitionBand
        visible: isFIR()
        axisX: frequencyAxisX
        axisY: magnitudeAxisY
        color: "gray"
        borderWidth: 0
        opacity: 0.8
    }

    LineSeries {
        id: frequencyResponseSeries
        axisX: frequencyAxisX
        axisY: magnitudeAxisY
        color: "mediumspringgreen"
        width: 2
    }

    function createBandLineSeries(from, to, y, parentObject) {
        return Qt.createQmlObject("import QtQuick; import QtCharts; LineSeries {
            XYPoint { x: " + from + "; y: " + y + "}
            XYPoint { x: " + to + "; y: " + y + "}
            }", parentObject)
    }

    function updateBandLineSeries() {
        var startPassFrequency = 0
        var endPassFrequency = backend.getCutoffFrequency()
        if (isHighPass()) {
            startPassFrequency = backend.getCutoffFrequency()
            endPassFrequency = backend.getCutoffFrequencyRangeTo()
        }

        passBand.upperSeries = createBandLineSeries(
                    startPassFrequency, endPassFrequency,
                    backend.getFrequencyResponseMaxValue(),
                    passBand)
        passBand.lowerSeries = createBandLineSeries(
                    startPassFrequency, endPassFrequency,
                    backend.getFrequencyResponseMinValue(),
                    passBand)

        var startTransitionFrequency = backend.getCutoffFrequency()
        var endTransitionFrequency = backend.getTransitionLength() + backend.getCutoffFrequency()
        if (isHighPass()) {
            startTransitionFrequency = backend.getCutoffFrequency() - backend.getTransitionLength()
            endTransitionFrequency = backend.getCutoffFrequency()
        }

        transitionBand.upperSeries = createBandLineSeries(
                    startTransitionFrequency, endTransitionFrequency,
                    backend.getFrequencyResponseMaxValue(),
                    transitionBand)
        transitionBand.lowerSeries = createBandLineSeries(
                    startTransitionFrequency, endTransitionFrequency,
                    backend.getFrequencyResponseMinValue(),
                    transitionBand)
    }

    function updateComponents() {
        frequencyAxisX.min = backend.getVisibleFrequencyFrom()
        frequencyAxisX.max = backend.getVisibleFrequencyTo()

        magnitudeAxisY.min = backend.getFrequencyResponseMinValue()
        magnitudeAxisY.max = backend.getFrequencyResponseMaxValue()

        transitionBand.visible = isFIR()

        updateBandLineSeries()

        backend.updateFrequencyResponse(frequencyResponseSeries)
    }

    Connections {
        target: backend
        function onCalculationCompleted() {
            updateComponents()
        }
    }

    Connections {
        target: backend
        function onControlsStateChanged() {
            updateComponents()
        }
    }
}
