import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    Label {
        id: mainLabel1
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "You should install the Meecast Daemon \npackage first for this feature.\nYou can find it here:"
    }
    Button {
        id: button
        anchors.top: mainLabel1.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "http://openrepos.net"
        onClicked: Qt.openUrlExternally("https://openrepos.net/content/vasvlad/meecast-daemon") 
    }

    onDone: {
        if (result == DialogResult.Accepted) {
        }
    }
}

