import Qt 4.7

BorderImage {
    signal clicked

    border {
        top: 10
        bottom: 10
        left: 10
        right: 10
    }
    source: buttonarea.pressed ? "gfx/button_pressed.png" : "gfx/button.png"

    MouseArea {
        id: buttonarea
        anchors.fill: parent
        onClicked: {
            parent.clicked();
        }
    }
}
