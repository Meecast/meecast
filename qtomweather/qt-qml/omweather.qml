import Qt 4.7

Rectangle {
    id: window
    width: 800
    height: 480
    Rectangle {
        id: buttons
        width: 800
        height: 80
        color: "blue"
        anchors.top: parent.top

    }
    Rectangle {
        width: 800
        height: 400
        anchors.top: buttons.bottom
        id: main


    Loader {
        id: uiloader

        function handleGoBack()
        {
            source = "";
            list.visible = true;
        }
        onItemChanged: {
            if (item && item.goBack)
                item.goBack.connect(handleGoBack);
        }
    }

    XmlListModel {
        id: xmlModel
        source: "datanew.xml"
        query: "/data/short/item"

        XmlRole {name: "dayname"; query: "dayname/string()"}
        XmlRole {name: "temperature_low"; query: "temperature_low/string()"}
        XmlRole {name: "temperature_high"; query: "temperature_high/string()"}
        XmlRole {name: "icon"; query: "icon/string()"}
        XmlRole {name: "id"; query: "@id/number()"}
    }
    Component {
        id: itemDelegate

        Item {
            id: day

            width: 200
            height: 190

            Rectangle {
                id: background_part
                anchors.fill: parent
                color: "#008080"


            }
            Text {text:  id}
            Text {
                id: day_name
                text: dayname
                color: "white"
                font.pointSize: 14
                anchors.left: parent.left
                anchors.leftMargin: 20
                //anchors.centerIn: parent
                //anchors.top: parent.top
            }

            Image {
                id: forecast_icon
                source: icon
                width: 80
                height: 80
                anchors.top: day_name.bottom
                //anchors.topMargin: 20
                //anchors.leftMargin: 5
                anchors.left: parent.left
            }
            Text {
                id: temperature
                text: temperature_low + " : " + temperature_high
                anchors.top: forecast_icon.top; anchors.topMargin: 10
                anchors.left: forecast_icon.right; anchors.leftMargin: 10
                color: "white"
                font.pointSize: 14
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    uiloader.source = "Details.qml";
                    list.visible = false;
                    uiloader.item.item_id = id;
                }
                hoverEnabled: true

            }

        }

    }

    GridView {
        id: list
        anchors.fill: parent
        cellWidth: 200; cellHeight: 190
        model: xmlModel
        delegate: itemDelegate

    }
    }
}
