import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Component {
    id: item_delegate
    Rectangle {
        id: item_style
        implicitHeight: 70
        implicitWidth: table.width

        border.width: 4
        border.color: "gray"
        radius: 4

        clip: true

        gradient: Gradient {
            GradientStop { position: 0.1; color: qsTr("#DCDCDC") }
            GradientStop { position: 0.5; color: qsTr("#F5F5F5") }
        }

        Rectangle {
            id: item_header
            anchors {
                left: count.right
                right: item_style.right
                top: item_style.top
            }

            implicitHeight: 32
            color: "transparent"

            Text {
                anchors.fill: item_header

                text: device_name
                font.pixelSize: 14
                font.bold: false
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Rectangle {
            id: count
            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }

            radius: 4
            clip: true
            color: "transparent"

            implicitWidth: 50
        }

        Rectangle {
            id: devices

            anchors {
                left: count.right
                right: item_style.right
                bottom: item_style.bottom
                top: item_header.bottom
            }

            color: "transparent"
            ColumnLayout {
                anchors.fill: parent
                spacing: 2

                Text {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    Layout.topMargin: 2
                    text: qsTr("<b>Address:</b> ") + address
                }

            }
        }

        MouseArea {
            id: ma_area
            anchors.fill: parent

            onClicked: {
                console.log("Cliсked: ", address)
                controller.pushData(address)
            }
        }
    }
}
