import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtCharts
import filter.designer.qmlcomponents

ApplicationWindow {
    id: window
    visible: true
    title: qsTr("Filter Designer")
    readonly property int margin: 11

    palette.window: "dimgray"
    palette.windowText: "white"
    palette.base: "black"
    palette.button: "black"
    palette.text: "white"
    palette.disabled.text: "dimgray"
    palette.buttonText: "white"
    palette.highlight: "darkorange"

    Component.onCompleted: {
        width = mainLayout.implicitWidth + 2 * margin
        height = mainLayout.implicitHeight + 2 * margin
    }
    minimumWidth: mainLayout.Layout.minimumWidth + 2 * margin
    minimumHeight: mainLayout.Layout.minimumHeight + 2 * margin

    function isFIR() {
        return filterType.currentValue === "FIR"
    }

    function createBandLineSeries(from, to, y, parentObject) {
        return Qt.createQmlObject("import QtQuick; import QtCharts; LineSeries {
            XYPoint { x: " + from + "; y: " + y + "}
            XYPoint { x: " + to + "; y: " + y + "}
            }", parentObject)
    }

    function updateBandLineSeries() {
        passBand.upperSeries = createBandLineSeries(
                    0, backend.getCutoffFrequency(),
                    backend.getFrequencyResponseMaxValue(),
                    passBand)
        passBand.lowerSeries = createBandLineSeries(
                    0, backend.getCutoffFrequency(),
                    backend.getFrequencyResponseMinValue(),
                    passBand)

        transitionBand.upperSeries = createBandLineSeries(
                    backend.getCutoffFrequency(),
                    backend.getTransitionLength() + backend.getCutoffFrequency(),
                    backend.getFrequencyResponseMaxValue(),
                    transitionBand)
        transitionBand.lowerSeries = createBandLineSeries(
                    backend.getCutoffFrequency(),
                    backend.getTransitionLength() + backend.getCutoffFrequency(),
                    backend.getFrequencyResponseMinValue(),
                    transitionBand)
    }

    Backend {
        id: backend
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: window.margin

        ColumnLayout {
            id: leftLayout
            Layout.minimumWidth: 200
            Layout.preferredWidth: 200

            GroupBox {
                Layout.fillWidth: true
                background: Rectangle {
                    radius: 0
                    border.width: 0
                    border.color: "gray"
                    color: "transparent"
                }

                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        text: qsTr("Sampling Rate (Hz)")
                    }
                    SpinBox {
                        id: samplingRate
                        Layout.fillWidth: true
                        editable: true
                        from: backend.getSamplingRateRangeFrom()
                        to: backend.getSamplingRateRangeTo()
                        value: backend.getSamplingRate()
                        onValueChanged: backend.setSamplingRate(value)
                    }
                }
            }

            GroupBox {
                Layout.fillWidth: true
                background: Rectangle {
                    radius: 0
                    border.width: 0
                    border.color: "gray"
                    color: "transparent"
                }

                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        text: qsTr("Cutoff Frequency (Hz)")
                    }
                    SpinBox {
                        id: cutoffFrequency
                        Layout.fillWidth: true
                        from: backend.getCutoffFrequencyRangeFrom()
                        to: backend.getCutoffFrequencyRangeTo()
                        editable: true
                        value: backend.getCutoffFrequency()
                        onValueChanged: backend.setCutoffFrequency(value)
                    }

                    Label {
                        text: qsTr("Pass")
                    }
                    ComboBox {
                        id: passType
                        Layout.fillWidth: true
                        model: ["Low Pass"]
                        onCurrentValueChanged: backend.setPassType(currentValue)
                    }

                    Label {
                        text: qsTr("Filter Type")
                    }
                    ComboBox {
                        id: filterType
                        Layout.fillWidth: true
                        model: ["FIR", "IIR"]
                        onCurrentValueChanged: backend.setFilterType(currentValue)
                    }

                    Label {
                        text: qsTr("Window")
                        visible: isFIR()
                    }
                    ComboBox {
                        id: windowType
                        Layout.fillWidth: true
                        model: ["Blackman", "Rectangular"]
                        visible: isFIR()
                        onCurrentValueChanged: backend.setWindowType(currentValue)
                    }

                    Label {
                        text: qsTr("Attenuation (dB)")
                        visible: isFIR()
                    }
                    RowLayout {
                        visible: isFIR()
                        Layout.fillWidth: true

                        TextField {
                            id: attenuationDBText
                            text: attenuationDBSlider.value
                            Layout.minimumWidth: 25
                            Layout.maximumWidth: 25
                            readOnly: true
                        }

                        Slider {
                            id: attenuationDBSlider
                            stepSize: 1
                            live: false
                            Layout.fillWidth: true
                            from: backend.getAttenuationDBRangeFrom()
                            to: backend.getAttenuationDBRangeTo()
                            value: backend.getAttenuationDB()
                            onValueChanged: backend.setAttenuationDB(value)
                        }
                    }

                    Label {
                        text: qsTr("Transition Length (Hz)")
                        visible: isFIR()
                    }
                    SpinBox {
                        id: transitionLength
                        enabled: useOptimalFilterSize.checked
                        visible: isFIR()
                        Layout.fillWidth: true
                        editable: true
                        from: backend.getTransitionLengthRangeFrom()
                        to: backend.getTransitionLengthRangeTo()
                        value: backend.getTransitionLength()
                        onValueChanged: backend.setTransitionLength(value)
                    }

                    CheckBox {
                        id: useOptimalFilterSize
                        text: qsTr("Optimal Filter Size")
                        visible: isFIR()
                        Layout.fillWidth: true
                        leftPadding: 0
                        checked: backend.isUseOptimalFilterSize()
                        onCheckedChanged: backend.setUseOptimalFilterSize(checked)
                    }

                    Label {
                        text: qsTr("Size")
                        visible: isFIR()
                    }
                    SpinBox {
                        id: filterSize
                        enabled: !useOptimalFilterSize.checked
                        Layout.fillWidth: true
                        editable: true
                        visible: isFIR()
                        from: backend.getFilterSizeRangeFrom()
                        to: backend.getFilterSizeRangeTo()
                        value: backend.getFilterSize()
                        onValueChanged: backend.setFilterSize(value)
                    }

                }
            }

            GroupBox {
                id: displayGroupBox
                Layout.fillWidth: true
                background: Rectangle {
                    radius: 0
                    border.width: 0
                    border.color: "gray"
                    color: "transparent"
                }

                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        text: qsTr("Visible Frequency Range")
                    }
                    RowLayout {
                        Layout.fillWidth: true

                        SpinBox {
                            id: visibleFrequencyFrom
                            Layout.fillWidth: true
                            editable: true
                            from: backend.getSamplingRateRangeFrom() / 2
                            to: visibleFrequencyTo.value - 1
                            value: backend.getVisibleFrequencyFrom()
                            onValueChanged: backend.setVisibleFrequencyFrom(value)
                        }

                        SpinBox {
                            id: visibleFrequencyTo
                            Layout.fillWidth: true
                            editable: true
                            from: visibleFrequencyFrom.value + 1
                            to: samplingRate.value / 2
                            value: backend.getVisibleFrequencyTo()
                            onValueChanged: backend.setVisibleFrequencyTo(value)
                        }

                    }
                }
            }

            Item {
                Layout.fillHeight: true
                Layout.minimumHeight: 50
            }

            Connections {
                target: backend
                function onControlsStateChanged() {
                    samplingRate.from = backend.getSamplingRateRangeFrom()
                    samplingRate.to = backend.getSamplingRateRangeTo()

                    cutoffFrequency.from = backend.getCutoffFrequencyRangeFrom()
                    cutoffFrequency.to = backend.getCutoffFrequencyRangeTo()

                    filterSize.value = backend.getFilterSize()
                    transitionLength.value = backend.getTransitionLength()

                    visibleFrequencyFrom.value = backend.getVisibleFrequencyFrom()
                    visibleFrequencyTo.value = backend.getVisibleFrequencyTo()
                }
            }
        }

        ColumnLayout {
            id: rightLayout

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
                    gridVisible: false
                    lineVisible: false
                    labelFormat: "%.0f"
                    labelsColor: "dimgray"
                    titleText: "Frequency (Hz)"
                }

                ValuesAxis {
                    id: magnitudeAxisY
                    min: -100
                    max: 0
                    gridVisible: false
                    lineVisible: false
                    labelsColor: "dimgray"
                    titleText: "Magnitude (dB)"
                }

                AreaSeries {
                    id: passBand
                    visible: isFIR()
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

                Connections {
                    target: backend
                    function onCalculationCompleted() {
                        frequencyAxisX.min = backend.getVisibleFrequencyFrom()
                        frequencyAxisX.max = backend.getVisibleFrequencyTo()

                        magnitudeAxisY.min = backend.getFrequencyResponseMinValue()
                        magnitudeAxisY.max = backend.getFrequencyResponseMaxValue()

                        updateBandLineSeries()

                        backend.updateFrequencyResponse(frequencyResponseSeries)
                    }
                }

                Connections {
                    target: backend
                    function onControlsStateChanged() {
                        frequencyAxisX.min = backend.getVisibleFrequencyFrom()
                        frequencyAxisX.max = backend.getVisibleFrequencyTo()

                        magnitudeAxisY.min = backend.getFrequencyResponseMinValue()
                        magnitudeAxisY.max = backend.getFrequencyResponseMaxValue()

                        updateBandLineSeries()

                        backend.updateFrequencyResponse(frequencyResponseSeries)
                    }
                }
            }


            RowLayout {

                ChartView {
                    id: filterCoefficients
                    title: "Filter Coefficients"
                    Layout.minimumWidth: 150
                    Layout.minimumHeight: 100
                    Layout.preferredWidth: 150
                    Layout.preferredHeight: 100
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: -margin
                    legend.visible: false
                    antialiasing: true
                    backgroundRoundness: 0
                    backgroundColor: "black"
                    titleColor: "white"
                    margins.top: 0
                    margins.left: margin
                    margins.right: margin
                    margins.bottom: margin
                    visible: isFIR()

                    ValuesAxis {
                        id: indexAxisX
                        min: 0
                        max: 1000
                        gridVisible: false
                        lineVisible: false
                        labelsVisible: false
                        labelFormat: "%.0f"
                        labelsColor: "dimgray"
                    }

                    ValuesAxis {
                        id: valueAxisY
                        min: -0.5
                        max: 1
                        gridVisible: false
                        lineVisible: false
                        labelsVisible: false
                        labelsColor: "dimgray"
                    }

                    LineSeries {
                        id: coefficientsSeries
                        axisX: indexAxisX
                        axisY: valueAxisY
                        color: "tomato"
                        width: 2
                    }

                    Connections {
                        target: backend
                        function onCalculationCompleted() {
                            indexAxisX.max = backend.getCoefficientsCount()
                            valueAxisY.min = backend.getCoefficientsMinValue()
                            valueAxisY.max = backend.getCoefficientsMaxValue()
                            backend.updateCoefficients(coefficientsSeries)
                        }
                    }
                }

                Flickable {
                    id: flickable
                    Layout.minimumHeight: 100
                    Layout.minimumWidth: 300
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    TextArea.flickable: TextArea {
                        id: coefficients
                        wrapMode: TextEdit.WordWrap
                        background: Rectangle {
                            color: "black"
                        }

                        color: "dimgray"

                        Connections {
                            target: backend
                            function onCalculationCompleted() {
                                coefficients.text = backend.getCoefficientsString()
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }

                Button {
                    id: copyCoefficientsToClipboard
                    Layout.preferredWidth: 50
                    Layout.minimumWidth: 50
                    Layout.preferredHeight: 100
                    Layout.minimumHeight: 100
                    Layout.fillHeight: true
                    icon.source: "/icons/qt/clipboard.png"
                    icon.color: "white"
                    palette.button: "dimgray"
                    onClicked: {
                        coefficients.selectAll()
                        coefficients.copy()
                    }
                }
            }
        }
    }
}
