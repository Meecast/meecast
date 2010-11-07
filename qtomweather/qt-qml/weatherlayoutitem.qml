import Qt 4.7

LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"

    Rectangle { color: "yellow"; anchors.fill: parent
        Image {
             id: forecast_icon
             source: Config.iconspath + "/" + Config.iconset+"/" + Forecast.icon + ".png"
             anchors.centerIn: parent
             anchors.left: parent.left
         }
        Text {
            id: temperature_high
            text: Forecast.temperature_high
            font.pointSize: 20
            //	color: "white"
            anchors.right: parent.right
            anchors.left: forecast_icon.right
            anchors.top: forecast_icon.top
            anchors.topMargin: 10
        }
        Text {
            id: temperature_low
            text: Forecast.temperature_low
            font.pointSize: 20
            //	color: "white"
            anchors.right: parent.right
            anchors.left: forecast_icon.right
            anchors.top: temperature_high.bottom
            anchors.topMargin: 10
        }

    }

    //Rectangle {
    //   width: 100; height: 100
    //   anchors.top: parent.top; anchors.right: parent.right
    //   color: "green"
    //}

}
