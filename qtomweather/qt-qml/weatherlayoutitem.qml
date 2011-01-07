import Qt 4.7
/*
LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"
*/

    Rectangle {
        width: 256
        height: 256
        color: "yellow";
        //anchors.fill: parent
        //id: area
        id: weatherlayoutitem
        Image {
            id: forecast_icon
            source: Config.iconspath + "/" + Config.iconset + "/" + Forecast.icon + ".png"
            anchors.centerIn: parent
            anchors.left: parent.left
        }
        Text {
            id: temperature_high
            text: Forecast.temperature_high + '°' +Config.temperatureunit
            font.pointSize: 20
            color: Config.fontcolor
            anchors.right: parent.right
            anchors.left: forecast_icon.right
            anchors.top: forecast_icon.top
            anchors.topMargin: 10
        }
        Text {
            id: temperature_low
            text: Forecast.temperature_low + '°' +Config.temperatureunit
            font.pointSize: 20
            color: Config.fontcolor
            //	color: "white"
            anchors.right: parent.right
            anchors.left: forecast_icon.right
            anchors.top: temperature_high.bottom
            anchors.topMargin: 10
        }

    }

//}
