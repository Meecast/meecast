import Qt 4.7

Rectangle {
    id: details

    color: "#008080"
    //width: 400
    //height: 400
    //anchors.fill: parent
    signal goBack()


    property int item_id: 0
    Text {text: item_id}

    XmlListModel {
        id: detailModel
        source: "/tmp/1.xml"
        query: "/data/item[@id='"+item_id+"']"

        XmlRole {name: "dayname"; query: "dayname/string()"}
        XmlRole {name: "temperature_low"; query: "temperature_low/string()"}
        XmlRole {name: "temperature_high"; query: "temperature_hi/string()"}
        XmlRole {name: "temperature"; query: "temperature/string()"}
        XmlRole {name: "icon"; query: "icon/string()"}
        XmlRole {name: "wind_speed"; query: "wind_speed/string()"}
        XmlRole {name: "wind_direction"; query: "wind_direction/string()"}
        XmlRole {name: "humidity"; query: "humidity/string()"}
        XmlRole {name: "ppcp"; query: "ppcp/string()"}
        XmlRole {name: "pressure"; query: "pressure/string()"}
        XmlRole {name: "description"; query: "description/string()"}
        XmlRole {name: "id_item"; query: "@id/number()"}
        //XmlRole {name: "current"; query:  "@current/boolean()"}
    }


    Component {
        id: itemDelegateFull
        Item {
            Image {
                id: forecast_icon
                source: icon
                anchors.top: details.top
                anchors.topMargin: 20
                anchors.leftMargin: 20
                anchors.left: details.left
            }
            Column {
                spacing: 5
                anchors.right: parent.right
                anchors.left: forecast_icon.right
                anchors.top: forecast_icon.top
                anchors.topMargin: 10

                Text {text: qsTr("Temperature") + ": " + (temperature) ? temperature : (temperature_low + " : " + temperature_high)}
                Text {text: description }
                Text {text: qsTr("Humidity") + ": " + humidity }
                Text {text: qsTr("Wind") + ": " + wind_direction}
                Text {text: qsTr("Speed") + ": " + wind_speed}
                Text {text: (wind_gust != "N/A") ?
                      (qsTr("Wind gust") + ": " + wind_gust) : ""}

                Text {text: (ppcp != "N/A") ?
                      (qsTr("Ppcp") + ": " + ppcp) : ""}
                Text {text: (pressure != "N/A") ?
                      (qsTr("Pressure") + ": " + pressure) : ""}

            }
        }
    }
    ListView {
        anchors.fill: parent
        model: detailModel
        delegate: itemDelegateFull
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked: details.goBack()
    }

}
