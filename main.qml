import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "controls" as Controls


ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("Bluetooth Client")

    Controls.MainView {
        anchors.fill: parent
    }
}
