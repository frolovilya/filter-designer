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

    Backend {
        id: backend
    }

    function isFIR() {
        return backend.getFilterType() === "FIR"
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.margins: window.margin

        Controls {}

        ColumnLayout {
            id: rightLayout

            FrequencyResponse {}
            FilterCoefficients {}
        }
    }
}
