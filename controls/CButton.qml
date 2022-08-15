import QtQuick 2.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15


Item {
    id: button

    property color btnColor: qsTr("#777")
    property color btnDownColor: qsTr("#006633")
    property color btnColorEnterd: qsTr("#1CAC78")
    property color btnTextColor: qsTr("#fff")

    property var hovered: ma_button.hover

    property int borderRadius: 4
    property string btnText: "Default btn text"

    property var clicked: () => {}
    property var pressed: () => {}
    property var released: () => {}

    Rectangle {
        id: btn_style
        anchors.fill: parent

        color: button.down ? btnDownColor : btnColor

        radius: borderRadius

        SequentialAnimation {
            id: color_entered
            PropertyAnimation {
                target: btn_style
                property: "color"
                to: button.btnColorEnterd
                duration: 50
            }
        }

        SequentialAnimation {
            id: color_exited
            PropertyAnimation {
                target: btn_style
                property: "color"
                to: btnColor
                duration: 500
            }
        }

        Text {
            anchors.fill: parent
            padding: 4

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: button.btnText
            font.family: qsTr("Arial")
            font.pointSize: 11
            color: button.btnTextColor
            wrapMode: Text.Wrap
        }
    }


    MouseArea {
        id: ma_button
        anchors.fill: parent

        property var hover: onHoveredChanged

        hoverEnabled: true

        onEntered: {
            color_entered.running = true
            btn_shadow.color = "b0000000"
        }

        onExited: {
            color_exited.running = true

            btn_shadow.color = "#fff"
        }

        onClicked: {
            button.clicked()
        }

        onPressed: {
            btn_style.color = button.btnDownColor

            button.pressed()
        }

        onReleased: {
            btn_style.color = button.btnColorEnterd
            button.released()
        }
    }

    DropShadow {
        id: btn_shadow
        anchors.fill: btn_style
        cached: true
        horizontalOffset: 0
        verticalOffset: 0
        spread: 0.1
        radius: 8.0
        samples: 24
        color: "#fff"
        smooth: true
        source: btn_style
    }
}
