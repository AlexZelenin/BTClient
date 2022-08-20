import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item {
    id: main_view

    RowLayout{
        id: layout
        anchors.fill: parent
        anchors.margins: 20

        Item {
            id: item
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop

            Layout.fillWidth: true
            Layout.fillHeight: true


            Column {
                spacing: 10
                CButton {
                    id: btn_scan

                    width: item.width
                    height: 35

                    btnText: "Поиск устройств"

                    onClicked: {
                        controller.startScanDevices();
                    }
                }
                CButton {
                    id: btn_clear

                    width: item.width
                    height: 35

                    btnText: "Очистить"

                    onClicked: {
                        devicemodel.clear()
                    }
                }

                CButton {
                    id: btn_scanStop

                    width: item.width
                    height: 35

                    btnText: "Остановить поиск"

                    onClicked: {
                        controller.stopScanDevices()
                    }
                }
            }
        }

        Component {
            id: highlight
            Rectangle {
                width: table.width;
                height: table.height
                color: "lightsteelblue"; radius: 5
            }
        }

        ListView {
            id: table

            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 1
            spacing: 1
            clip: false

            headerPositioning: ListView.OverlayHeader

            function size() {
                return table.count
            }

            model: devicemodel
            delegate: DeviceItem {}

            highlight: highlight
            highlightFollowsCurrentItem: false
            focus: true
        }
    }

}
