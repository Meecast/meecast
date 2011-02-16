import Qt 4.7

Item {
    id: combobox

    width: 100
    height: 32

    property alias model: elements.model
    property string text
    property bool pressed: false


    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        //onPressed: {combobox.pressed = true; elements.opacity = elements.opacity ? 0 : 1;}
        //onReleased: combobox.pressed = false
    }

    Component {
        id: listdelegate
        Item {
            width: combobox.width
            height: combobox.height
            Column {
                Text {
                    text: "1243 " + content
                }
            }
            /*
            function selectItem(index)
            {
                combobox.text = elements.model.get(index).content;

            }
            MouseArea {
                //anchors.fill: parent
                //hoverEnabled: true

                onEntered: {
                    elements.currentIndex = index;
                }
                onClicked: selectItem(index);
            }*/
        }
    }
    ListView {
        id: elements
        width: 100
        height: 200
        //opacity: 0
        delegate: listdelegate
        anchors.fill: parent
        focus: true
        highlight: Rectangle {color: "lightsteelblue"; radius: 5}
    }



}
