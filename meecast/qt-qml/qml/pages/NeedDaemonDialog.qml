import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    Label {
        id: mainLabel
        text: "You need install package Meecast Daemon from http://openrepos.net to activate this feature"
    }
    onDone: {
        if (result == DialogResult.Accepted) {
        }
    }
}

