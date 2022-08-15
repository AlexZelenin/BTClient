import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item {
    id: main_view

    RowLayout{
        anchors.fill: parent

        Rectangle {
            id: left_item
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "green"

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

        Rectangle {
            id: right_item
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "grey"
        }
    }

}
