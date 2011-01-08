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
            color: "yellow"
            opacity: 0.7
            //anchors.fill: parent
            //id: area
            id: weatherlayoutitem
            property int item_id: id
            Image {
                id: forecast_icon
                //source: Config.iconspath + "/" + Config.iconset + "/" + Forecast.icon + ".png"
                source: Config.iconspath + "/" + Config.iconset + "/" + pict
                //anchors.centerIn: parent
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.leftMargin: 20
                anchors.left: parent.left
            }
            Text {
                id: temperature_high
                //text: Forecast.temperature_high + '°' +Config.temperatureunit
                text: temp_high + '°' +Config.temperatureunit
                font.pointSize: 20
                color: Config.fontcolor
                anchors.right: parent.right
                anchors.left: forecast_icon.right
                anchors.top: forecast_icon.top
                anchors.topMargin: 10
            }
            Text {
                id: temperature_low
                //text: Forecast.temperature_low + '°' +Config.temperatureunit
                text: temp_low + '°' +Config.temperatureunit
                font.pointSize: 20
                color: Config.fontcolor
                //	color: "white"
                anchors.right: parent.right
                anchors.left: forecast_icon.right
                anchors.top: temperature_high.bottom
                anchors.topMargin: 10
            }
            MouseArea {
                anchors.fill: parent
                onClicked: details.show(parent.item_id)
                hoverEnabled: true
                onEntered: parent.opacity = 1
                onExited: parent.opacity = 0.7
               }
        }


    }
    ListModel {
        id: itemModel
        ListElement {
            pict: "0.png"
            temp_high: "-4°C"
            temp_low: "N/A"
        }
        ListElement {
            pict: "0.png"
            temp_high: "0°C"
            temp_low: "2°C"
        }
    }
    GridView {
        id: list
        anchors.fill: parent
        cellWidth: 220; cellHeight: 200
        //model: itemModel
        model: Forecast_model
        delegate: itemDelegate
    }

    Details {
        id: details
    }
}
