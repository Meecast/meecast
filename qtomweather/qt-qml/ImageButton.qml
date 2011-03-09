import Qt 4.7

Rectangle {
    width: 64
    height: 64

    color: "#00000000"

    property alias imagefile: pict.source

    signal clicked

    id: container

    Image {
        id: pict
        width: 64
        height: 64
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {container.clicked()}
    }

    states: State {
        name: "pressed"
        when: mouseArea.pressed
        PropertyChanges {target: pict; width: 60; height: 60}
    }
}
