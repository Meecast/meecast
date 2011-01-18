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
        Text {text: qsTr("Temperature") + ": " + Forecast_model.getRow(details.item_id, "temp_low") + " .. " +
              Forecast_model.getRow(details.item_id, "temp_high")}
        /*Text {text: (Forecast_model.getRow(details.item_id, "flike") != "N/A") ?
              (qsTr("Flike") + ": " + Forecast_model.getRow(details.item_id, "flike")) : ""}*/
        Text {text: Forecast_model.getRow(details.item_id, "description") }
        Text {text: qsTr("Humidity") + ": " + Forecast_model.getRow(details.item_id, "humidity") }
        Text {text: qsTr("Wind direction") + ": " + Forecast_model.getRow(details.item_id, "wind_direction")}
        Text {text: qsTr("Wind speed") + ": " + Forecast_model.getRow(details.item_id, "wind_speed")}
        Text {text: (Forecast_model.getRow(details.item_id, "wind_gust") != "N/A") ?
              (qsTr("Wind gust") + ": " + Forecast_model.getRow(details.item_id, "wind_gust")) : ""}

        Text {text: (Forecast_model.getRow(details.item_id, "ppcp") != "N/A") ?
              (qsTr("Ppcp") + ": " + Forecast_model.getRow(details.item_id, "ppcp") + "%") : ""}
        Text {text: (Forecast_model.getRow(details.item_id, "pressure") != "N/A") ?
              (qsTr("Pressure") + ": " + Forecast_model.getRow(details.item_id, "pressure")) : ""}
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: hide()
    }
}
