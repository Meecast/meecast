import Qt 4.7

LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"

    Rectangle {
        color: "yellow";
        anchors.fill: parent
        id: area
        Image {
             id: forecast_icon
             source: Config.iconspath + "/" + Config.iconset + "/" + Forecast.icon + ".png"
             anchors.centerIn: parent
             anchors.left: parent.left
             Behavior on width
             {
                 NumberAnimation {duration: 500}
             }
             Behavior on height
             {
                 NumberAnimation {duration: 500}
             }
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
        function hide()
        {
            forecast_icon.width = 0
            forecast_icon.height = 0

        }
        function show()
        {
            forecast_icon.width = 128
            forecast_icon.height = 128
        }

    }

    Rectangle {
        id: button
        width: 50; height: 50
        anchors.top: parent.top; anchors.left: parent.left
        color: "green"
        MouseArea
        {
            anchors.fill: parent
            id: mouseArea
            onClicked: callback()
        }
        states: [
            State {
                name: "hovered"
                when:  mouseArea.pressed
                PropertyChanges {
                    target: button
                    opacity: 1

                }
            },
            State {
                name: "normal"
                when:  mouseArea.pressed == false
                PropertyChanges {
                    target: button
                    opacity: 0.7

                }
            }
        ]
        Behavior on opacity
        {
            NumberAnimation {duration: 100}
        }
        Behavior on width
        {
            NumberAnimation {duration: 100}
        }
        Behavior on height
        {
            NumberAnimation {duration: 100}
        }

    }
    function callback()
    {
        temperature_high.text = Forecast.inc_temperature_high() + '°' +Config.temperatureunit;
        if (forecast_icon.width > 0){
            forecast_icon.width = 0;
            forecast_icon.height = 0;
        }else {
            forecast_icon.width = 128;
            forecast_icon.height = 128;
        }
        //area.hide();
        //area.show()
    }

}
