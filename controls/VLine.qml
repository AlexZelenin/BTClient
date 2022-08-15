import QtQuick 2.15
import QtQuick.Controls 2.15


Item {
    id: vline

    property int cwidth: 1
    property int cheigth: 10

    width: cwidth
    height: cheigth

    Rectangle {
        id: vline_style
        anchors.fill: parent
        color: "gray"
    }
}
