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
                    backgroundRoundness: 0
                    margins.left: 0
                    margins.right: 0
                    margins.top: 0
                    margins.bottom: 0
                    Layout.margins: -margin

                    LineSeries {
                        XYPoint { x: 0; y: 0 }
                        XYPoint { x: 1; y: -1 }
                        XYPoint { x: 2; y: -2 }
                        XYPoint { x: 3; y: -3 }
                        XYPoint { x: 4; y: -4 }
                        XYPoint { x: 5; y: -5 }
                    }
                }

                ChartView {
                    title: "Filter Coefficients"
                    Layout.minimumWidth: 300
                    Layout.minimumHeight: 300
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    legend.visible: false
                    backgroundRoundness: 0
                    margins.left: 0
                    margins.right: 0
                    margins.top: 0
                    margins.bottom: 0
                    Layout.margins: -margin
                    visible: isFIR()

                    LineSeries {
                        XYPoint { x: 0; y: 0 }
                        XYPoint { x: 1; y: -1 }
                        XYPoint { x: 2; y: -2 }
                        XYPoint { x: 3; y: -3 }
                        XYPoint { x: 4; y: -4 }
                        XYPoint { x: 5; y: -5 }
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
                    text: backend.getSamplingRate()
                    wrapMode: TextEdit.WordWrap
                    background: Rectangle { color: "white" }

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
