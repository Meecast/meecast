import Qt 4.7
/*
LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"
*/
Rectangle {
    width: 600
    height: 600


    Component {
        id: itemDelegate
        Rectangle {
            width: 256
            height: 256
            color: "yellow";
            //anchors.fill: parent
            //id: area
            id: weatherlayoutitem
            Image {
                id: forecast_icon
                //source: Config.iconspath + "/" + Config.iconset + "/" + Forecast.icon + ".png"
                source: Config.iconspath + "/" + Config.iconset + "/" + pict
                anchors.centerIn: parent
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
        cellWidth: 256; cellHeight: 256
        //model: itemModel
        model: Forecast_model
        delegate: itemDelegate
    }


}
