import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    Label {
        id: mainLabel1
        anchors.verticalCenter: parent.verticalCenter
        text: "You should install the \nMeecast Daemon package \n first for this feature."
    }
    Label {
        id: mainLabel2
        anchors.verticalCenter: parent.verticalCenter
        text: "You can find it here"
    }
    Button {
        id: button
        anchors.verticalCenter: parent.verticalCenter
        text: "http://openrepos.net"
        onClicked: Qt.openUrlExternally("http://openrepos.net") 
    }

    onDone: {
        if (result == DialogResult.Accepted) {
        }
    }
}

