import Qt 4.7

Rectangle {
    id: details
    color: "yellow"
    //width: 600
    //height: 400
    property int item_id: 0
    function show(id)
    {
        details.opacity = 1;
        details.item_id = id;
    }
    function hide()
    {
        details.opacity = 0;
    }

    opacity: 0
    width: parent.width
    height: parent.height

    visible: opacity > 0
    Text{text: "id = " + item_id}
    Image {
        id: icon
        source: Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getRow(details.item_id, "pict")
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.left: parent.left
    }
    Column {
        spacing: 5
        anchors.right: parent.right
        anchors.left: icon.right
        anchors.top: icon.top
        anchors.topMargin: 10
        Text {text: "Температура: " + Forecast_model.getRow(details.item_id, "temp_high")}
        Text {text: "Влажность: " + Forecast_model.getRow(details.item_id, "humidity") }
        Text {text: "Ветер: " + Forecast_model.getRow(details.item_id, "wind_direction")}
        Text {text: "Скорость: " + Forecast_model.getRow(details.item_id, "wind_speed")}
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: hide()
    }
}
