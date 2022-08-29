import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Styles 1.4


Dialog {
    id: dialog

    property int prgs: 0
    property int maxValue: 0

    visible: false
    title: "File transfer progress"

    Connections {
        target: controller

        function onTransferProgress(bytes, max) {
            pb.to = max;
            pb.value = bytes;
        }
    }

    contentItem: Rectangle {
        id: pstyle
        color: "transparent"
        implicitWidth: 400
        implicitHeight: 100

        ProgressBar {
            id: pb
            width: parent.width
            height: 35
            //value: dialog.prgs
            from: 0
            //to: dialog.maxValue

        }
    }

    CButton {
        width: 150
        implicitHeight: 35
        anchors.top: pb.bottom
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        btnText: "Закрыть"

        onClicked: {
            dialog.close()
        }
    }
}
