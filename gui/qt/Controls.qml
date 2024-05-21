import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtCharts
import filter.designer.qmlcomponents

ColumnLayout {
    id: leftLayout
    Layout.minimumWidth: 200
    Layout.preferredWidth: 200

    GroupBox {
        id: samplingGroupBox
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
        id: filterGroupBox
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
                id: cutoffFrequencyLabel
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
                id: passTypeLabel
                text: qsTr("Pass")
            }
            ComboBox {
                id: passType
                Layout.fillWidth: true
                model: backend.getPassTypes()
                onCurrentValueChanged: backend.setPassType(currentValue)
            }

            Label {
                id: filterTypeLabel
                text: qsTr("Filter Type")
            }
            ComboBox {
                id: filterType
                Layout.fillWidth: true
                model: backend.getFilterTypes()
                onCurrentValueChanged: backend.setFilterType(currentValue)
            }

            Label {
                id: windowTypeLabel
                text: qsTr("Window")
                visible: isFIR()
            }
            ComboBox {
                id: windowType
                Layout.fillWidth: true
                model: backend.getWindowTypes()
                visible: isFIR()
                onCurrentValueChanged: backend.setWindowType(currentValue)
            }

            Label {
                id: attenuationDBLabel
                text: qsTr("Attenuation (dB)")
                visible: isFIR()
            }
            RowLayout {
                id: attenuationDBControls
                visible: isFIR()
                Layout.fillWidth: true

                SpinBox {
                    id: attenuationDBSpinner
                    Layout.preferredWidth: 50
                    editable: true
                    from: backend.getAttenuationDBRangeFrom()
                    to: backend.getAttenuationDBRangeTo()
                    value: backend.getAttenuationDB()
                    onValueChanged: backend.setAttenuationDB(value)
                }

                Slider {
                    id: attenuationDBSlider
                    stepSize: 1
                    live: false
                    Layout.fillWidth: true
                    Layout.minimumWidth: 50
                    from: attenuationDBSpinner.from
                    to: attenuationDBSpinner.to
                    value: attenuationDBSpinner.value
                    onValueChanged: {
                        if (attenuationDBSpinner.value !== value) {
                            attenuationDBSpinner.value = value
                        }
                    }
                }
            }

            Label {
                id: transitionLengthLabel
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
                id: filterSizeLabel
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
                id: visibleFrequencyRangeLabel
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

            RowLayout {
                Layout.fillWidth: true

                Button {
                    id: fullVisibleFrequencyRange
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 25
                    Layout.fillWidth: true
                    text: "Full"
                    palette.button: "dimgray"
                    onClicked: {
                        backend.setVisibleFrequencyFrom(1)
                        backend.setVisibleFrequencyTo(
                                    backend.getSamplingRate() / 2)
                    }
                }

                Button {
                    id: visibleFrequencyRangeToCutoffX4
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 25
                    Layout.fillWidth: true
                    text: "Cutoff x4"
                    palette.button: "dimgray"
                    onClicked: {
                        if (backend.getPassType() === "Low Pass") {
                            backend.setVisibleFrequencyFrom(1)
                            backend.setVisibleFrequencyTo(
                                        backend.getCutoffFrequency() * 4)
                        } else if (backend.getPassType() === "High Pass") {
                            backend.setVisibleFrequencyFrom(
                                        backend.getSamplingRate() / 2
                                        - (backend.getSamplingRate() / 2 - backend.getCutoffFrequency()) * 4)
                            backend.setVisibleFrequencyTo(
                                        backend.getSamplingRate() / 2)
                        }
                    }
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
            windowTypeLabel.visible = isFIR()
            windowType.visible = isFIR()
            attenuationDBLabel.visible = isFIR()
            attenuationDBControls.visible = isFIR()
            transitionLengthLabel.visible = isFIR()
            transitionLength.visible = isFIR()
            useOptimalFilterSize.visible = isFIR()
            filterSizeLabel.visible = isFIR()
            filterSize.visible = isFIR()

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
