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

    Backend {
        id: backend
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: window.margin

        ColumnLayout {
            id: leftLayout

            Label {
                text: qsTr("Sampling Rate")
            }
            SpinBox {
                id: samplingRate
                from: backend.samplingRateFrom
                to: backend.samplingRateTo
                editable: true
                value: backend.defaultSamplingRate
                onValueChanged: backend.setSamplingRate(value)
            }

            Label {
                text: qsTr("Cutoff Frequency")
            }
            SpinBox {
                id: cutoffFrequency
                from: backend.cutoffFrequencyFrom
                to: backend.cutoffFrequencyTo
                editable: true
                value: backend.defaultCutoffFrequency
                onValueChanged: backend.setCutoffFrequency(value)
            }

            Label {
                text: qsTr("Pass")
            }
            ComboBox {
                id: passType
                model: ["Low Pass"]
                onCurrentValueChanged: backend.setPassType(currentValue)
            }

            Label {
                text: qsTr("Filter Type")
            }
            ComboBox {
                id: filterType
                model: ["FIR", "IIR"]
                onCurrentValueChanged: backend.setFilterType(currentValue)
            }

            Label {
                text: qsTr("Window")
                visible: isFIR()
            }
            ComboBox {
                id: windowType
                model: ["Blackman", "Rectangular"]
                visible: isFIR()
                onCurrentValueChanged: backend.setWindowType(currentValue)
            }


            Label {
                text: qsTr("Filter Size")
                visible: isFIR()
            }
            SpinBox {
                id: filterSize
                from: backend.filterSizeFrom
                to: backend.filterSizeTo
                value: backend.defaultFilterSize
                editable: true
                visible: isFIR()
                onValueChanged: backend.setFilterSize(value)
            }

            Item {
                Layout.fillHeight: true
            }
        }

        ColumnLayout {
            id: rightLayout

            RowLayout {

                ChartView {
                    id: frequencyResponse
                    title: "Frequency Response"
                    Layout.minimumWidth: 500
                    Layout.minimumHeight: 300
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    legend.visible: false
                    antialiasing: true
                    backgroundRoundness: 0
                    backgroundColor: "black"
                    titleColor: "white"
                    margins.left: 0
                    margins.right: 0
                    margins.top: 0
                    margins.bottom: 0
                    Layout.margins: -margin

                    ValuesAxis {
                        id: frequencyAxisX
                        min: 0
                        max: 1000
                        gridVisible: false
                        lineVisible: false
                        labelFormat: "%.0f"
                        labelsColor: "dimgray"
                    }

                    ValuesAxis {
                        id: magnitudeAxisY
                        min: -100
                        max: 0
                        gridVisible: false
                        lineVisible: false
                        labelsColor: "dimgray"
                    }

                    LineSeries {
                        id: frequencyResponseSeries
                        axisX: frequencyAxisX
                        axisY: magnitudeAxisY
                        color: "springgreen"
                        width: 2
                    }

                    Connections {
                        target: backend
                        function onCalculationCompleted() {
                            frequencyAxisX.max = backend.getFrequencyResponseBinsCount()
                            magnitudeAxisY.min = backend.getFrequencyResponseMinValue()
                            magnitudeAxisY.max = backend.getFrequencyResponseMaxValue()
                            backend.updateFrequencyResponse(frequencyResponseSeries)
                        }
                    }
                }

                ChartView {
                    id: filterCoefficients
                    title: "Filter Coefficients"
                    Layout.minimumWidth: 300
                    Layout.minimumHeight: 300
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    legend.visible: false
                    antialiasing: true
                    backgroundRoundness: 0
                    backgroundColor: "black"
                    titleColor: "white"
                    margins.left: 0
                    margins.right: 0
                    margins.top: 0
                    margins.bottom: 0
                    Layout.margins: -margin
                    visible: isFIR()

                    ValuesAxis {
                        id: indexAxisX
                        min: 0
                        max: 1000
                        gridVisible: false
                        lineVisible: false
                        labelFormat: "%.0f"
                        labelsColor: "dimgray"
                    }

                    ValuesAxis {
                        id: valueAxisY
                        min: -0.5
                        max: 1
                        gridVisible: false
                        lineVisible: false
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
            }

            Flickable {
                id: flickable
                Layout.minimumHeight: 50
                Layout.fillHeight: true
                Layout.fillWidth: true

                TextArea.flickable: TextArea {
                    id: coefficients
                    wrapMode: TextEdit.WordWrap
                    background: Rectangle { color: "black" }

                    color: "dimgray"

                    Connections {
                        target: backend
                        function onCalculationCompleted() {
                            coefficients.text = backend.getCoefficientsString();
                        }
                    }
                }

                ScrollBar.vertical: ScrollBar {}
            }
        }

    }
}
