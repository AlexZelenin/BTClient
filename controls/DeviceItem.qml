import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

Component {
    id: device_item

    Rectangle {
        id: item_style
        implicitHeight: 150
        implicitWidth: table.width

        border.width: 4
        border.color: "gray"
        radius: 4
        clip: true
        color: qsTr("#DCDCDC")

        Rectangle { // Header
            id: item_header
            color: "#9b9b9b"
            anchors {
                left: item_style.left
                right: item_style.right
                top: item_style.top
            }
            anchors.margins: 5

            implicitHeight: 45

            Connections {
                target: controller

                function onOpenFile(str) {
                    files.text = str;
                }

                function onSockConnect() {
                    btn_connect.enabled = false
                }
            }

            RowLayout {
                anchors.fill: item_header
                anchors.margins: 5
                spacing: 2
                Column {
                    Text {
                        text: qsTr("<b>Device name:</b> ") + device_name
                        font.pixelSize: 14
                        font.bold: false
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Text {
                        font.pixelSize: 14
                        font.bold: false
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("<b>Address:</b> ") + address
                    }
                }

                CButton {
                    id: btn_connect
                    implicitWidth: 100
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                    btnText: "Подключить"
                    btnColor: "#dcdcdc"
                    onClicked: {
                        //pin_confirm.run()
                        controller.startConnect(address)
                    }
                }
            }
        } // Header

        Rectangle {
            id: item_control

            anchors {
                top: item_header.bottom
                left: item_style.left
                right: item_style.right
                bottom: item_style.bottom
            }

            anchors.margins: 5

            color: "transparent"

            Progress {
                id: transferDialog

            }

            ColumnLayout {
                id: clm_lay
                anchors.fill: item_control

                Row {
                    Text {
                        id: status
                        text: qsTr("")
                    }
                }

                RowLayout {
                    id: row
                    spacing: 5

                    TextField {
                        id: textInput
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignRight
                        Layout.leftMargin: 5

                        implicitHeight: 35
                        text: qsTr("")
                        font.pixelSize: 14
                        placeholderText: "Bytes"


                        background: Rectangle {
                            anchors.fill: parent
                            color: "#fff"
                            border.width: 1
                            border.color: "green"
                            radius: 4
                        }
                    }

                    CButton {
                        id: btn_send

                        Layout.rightMargin: 5

                        width: 100
                        height: 35
                        btnText: "Отправить"
                        onClicked: {
                            controller.sendData(textInput.text)
                        }
                    }
                }

                RowLayout {
                    id: row2

                    spacing: 5
                    CButton {
                        width: 100
                        height: 35
                        btnText: "Открыть файл"

                        Layout.leftMargin: 5

                        onClicked:  {
                            controller.fileSelect();
                        }
                    }

                    TextField {
                        id: files
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignRight
                        implicitHeight: 35
                        text: qsTr("")
                        font.pixelSize: 14
                        readOnly: true

                        background: Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.width: 1
                            border.color: "green"
                            radius: 4
                        }
                    }

                    CButton {
                        id: btn_send_file

                        Layout.rightMargin: 5

                        width: 100
                        height: 35
                        btnText: "Отправить"
                        onClicked: {
                            controller.sendFile(address, files.text)
                            transferDialog.open()
                        }
                    }


                }
            }

        }
    }
}

