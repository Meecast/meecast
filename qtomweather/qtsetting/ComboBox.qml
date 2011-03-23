import Qt 4.7

Item {
    id: combobox

    width: 100
    height: 50

    property alias model: elements.model
    property alias text: currentText.text
    property bool pressed: false
    property string key: ""

    signal clicked;

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPressed: {
            combobox.pressed = true;
            combobox.height = elements.opacity ? 50 : 250;
            elements.opacity = elements.opacity ? 0 : 1;
        }
        //onReleased: combobox.pressed = false
    }
    Column{
        //spacing: 1

        Text {
            width: 100
            height: 50
            id: currentText
            //text: parent.text
        }


        ListView {
            id: elements
            width: 100
            height: 200
            opacity: 0
            delegate: listdelegate
            //delegate: Text { text: content}
            //anchors.fill: parent
            //focus: true
            highlight: Rectangle {color: "lightsteelblue"; radius: 5}
        }
    }

    Component {
        id: listdelegate
        Item {
            width: 100
            height: 32
            Column {
                Text {
                    text: "1243 " + content
                }
            }

            function selectItem(index)
            {
                combobox.text = elements.model.get(index).content;
                combobox.key = elements.model.get(index).key;
                //console.log(elements.model.get(index).content + " " + elements.model.get(index).key);
                combobox.height = 50;
                elements.opacity = 0;
                combobox.clicked();
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    //console.log(index);
                    elements.currentIndex = index;
                }
                onClicked: selectItem(index);

            }
        }
    }


}
