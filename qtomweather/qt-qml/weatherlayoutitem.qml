import Qt 4.7
/*
LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"
*/
Rectangle {
    width: 660
    height: 400


    Component {
        id: itemDelegate
        Rectangle {
            width: 220
            height: 200
            //color: !current ? "yellow" : "green"
            opacity: 0.7
            //anchors.fill: parent
            //id: area
            id: weatherlayoutitem
            Text {
                id: day_name
                text: date
                color: Config.fontcolor
                anchors.left: parent.left
                anchors.leftMargin: 50
                //anchors.centerIn: parent
                //anchors.top: parent.top
            }
            Image {
                id: forecast_icon
                source: Config.iconspath + "/" + Config.iconset + "/" + pict
                anchors.top: day_name.bottom
                //anchors.topMargin: 20
                //anchors.leftMargin: 5
                anchors.left: parent.left
            }

            Text {
                id: temperature
                text: ((temp_low != "N/A") ? (temp_low + '°' +Config.temperatureunit + " .. ") : "") +
                        ((temp_high != "N/A") ? (temp_high + '°' +Config.temperatureunit) : "")
                font.pointSize: 14
                color: Config.fontcolor
                //anchors.right: parent.right
                anchors.left: forecast_icon.right
                anchors.top: day_name.bottom
                anchors.topMargin: 10
            }
            Text {
                id: desc
                text: description
                font.pointSize: 14
                color: Config.fontcolor
                anchors.top: forecast_icon.bottom
                anchors.left: parent.left
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    details.show(index)
                }
                hoverEnabled: true
                onEntered: parent.opacity = 1
                onExited: parent.opacity = 0.7
               }
        }


    }

    GridView {
        id: list
        anchors.fill: parent
        cellWidth: 220; cellHeight: 200
        model: Forecast_model
        delegate: itemDelegate
    }

    Details {
        id: details
    }
}
