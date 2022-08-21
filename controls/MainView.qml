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
                        controller.startDiscovery();
                        busy_img.playing = true
                        busy_img.paused = false
                        busy_img.visible = true
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
                        controller.stopButton()
                        busy_img.playing = false
                        busy_img.paused = true
                        busy_img.visible = false
                    }
                }

                AnimatedImage {
                    id: busy_img
                    source: "qrc:/busy.gif"
                    Layout.alignment: Qt.AlignHCenter
                    width: 30
                    height: 30
                    playing: false
                    visible: false

                    Text {
                        id: info_text
                        anchors.left: busy_img.right
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 10
                        text: qsTr("Идет поиск устройств ...")
                        verticalAlignment: Qt.AlignBottom
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
