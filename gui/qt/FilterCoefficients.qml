import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtCharts
import filter.designer.qmlcomponents

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
            font.family: "Menlo"
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

    Connections {
        target: backend
        function onControlsStateChanged() {
            filterCoefficients.visible = isFIR()
        }
    }
}
