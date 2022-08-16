import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item {
    id: main_view

    RowLayout{
        anchors.fill: parent

        Item {
            id: left_item
            Layout.fillWidth: true
            Layout.fillHeight: true

            CButton {
                id: btn_scan
                width: 250
                height: 35
                btnText: "Поиск устройств"

                clicked: () => {
                             controller.startScanDevices();
                         }
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
            delegate: DeviceItemDelegate {}

            highlight: highlight
            highlightFollowsCurrentItem: false
            focus: true
        }
    }

}
