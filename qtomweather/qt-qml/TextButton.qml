import Qt 4.7

Rectangle {
    id: container

    property  alias text: label.text

    signal clicked

    width: label.width + 20
    height: 64

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {container.clicked()}
    }

    Text {
        id: label
        anchors.centerIn: parent
        font.pointSize: 16
    }

    states: State {
        name: "pressed"
        when: mouseArea.pressed
        PropertyChanges {target: label; opacity: 0.7}
    }

}
