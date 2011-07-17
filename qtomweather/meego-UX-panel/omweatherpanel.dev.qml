import Qt 4.7

Rectangle {
    id: rect
    width: 650
    height: 900
    color: "#008080"



            Loader {
                id: uiloader
                anchors.fill: parent
                function handleGoBack()
                {
                    source = "";
                    columnlist.visible = true;
                }
                onItemChanged: {
                    if (item && item.goBack)
                        item.goBack.connect(handleGoBack);
                }
            }
            XmlListModel {
                id: stationModel
                source: "../test/qmldata.xml"
                query: "/data/station"

                XmlRole {name:  "id"; query: "@id/string()"}
                XmlRole {name:  "name"; query: "@name/string()"}

            }
            XmlListModel {
                id: currentxmlModel
                source: "../test/qmldata.xml"
                query: "/data/station[1]/item[@current='true']"

                XmlRole {name: "dayname"; query: "dayname/string()"}
                XmlRole {name: "temperature_low"; query: "temperature_low/string()"}
                XmlRole {name: "temperature_high"; query: "temperature_hi/string()"}
                XmlRole {name: "temperature"; query: "temperature/string()"}
                XmlRole {name: "icon"; query: "icon/string()"}
                XmlRole {name: "wind_speed"; query: "wind_speed/string()"}
                XmlRole {name: "wind_direction"; query: "wind_direction/string()"}
                XmlRole {name: "humidity"; query: "humidity/string()"}
                XmlRole {name: "description"; query: "description/string()"}
                XmlRole {name: "id_item"; query: "@id/number()"}
                //XmlRole {name: "current"; query:  "@current/boolean()"}
            }

            XmlListModel {
                id: xmlModel
                source: "../test/qmldata.xml"
                query: "/data/station[1]/item[not(@current)]"

                XmlRole {name: "dayname"; query: "dayname/string()"}
                XmlRole {name: "temperature_low"; query: "temperature_low/string()"}
                XmlRole {name: "temperature_high"; query: "temperature_hi/string()"}
                XmlRole {name: "temperature"; query: "temperature/string()"}
                XmlRole {name: "icon"; query: "icon/string()"}
                XmlRole {name: "id_item"; query: "@id/number()"}
            }
            Component {
                id: itemDelegate
                Item {
                    id: day
                    width: list.cellWidth
                    height: list.cellHeight

                    Rectangle {
                        id: background_part
                        anchors.fill: parent
                        color: "#008080"


                    }
                    Text {text:  id_item}
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
                        id: temp
                        text: (temperature) ? temperature : (temperature_low + " : " + temperature_high)
                        anchors.top: forecast_icon.top; anchors.topMargin: 10
                        anchors.left: forecast_icon.right; anchors.leftMargin: 10
                        color: "white"
                        font.pointSize: 14
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            uiloader.source = "Details.qml";
                            //list.visible = false;
                            columnlist.visible = false
                            uiloader.item.item_id = id_item;
                            //uiloader.item.width = frontItemOmweather.width
                            //uiloader.item.height = frontItemOmweather.height
                        }
                        hoverEnabled: true

                    }

                }

            }

            Component {
                id: itemDelegateCurrent
                Item {
                    Image {
                        id: forecast_icon
                        source: icon
                        anchors.top: parent.top
                        anchors.topMargin: 20
                        anchors.leftMargin: 20
                        anchors.left: parent.left
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
                        /*
                        Text {text: (wind_gust != "N/A") ?
                              (qsTr("Wind gust") + ": " + wind_gust) : ""}

                        Text {text: (ppcp != "N/A") ?
                              (qsTr("Ppcp") + ": " + ppcp) : ""}
                        Text {text: (pressure != "N/A") ?
                              (qsTr("Pressure") + ": " + pressure) : ""}
                        */
                    }
                }
            }
            property int current_station: 0;

            Column {
                id: columnlist
                anchors.fill: parent

                Rectangle {
                    id: station
                    width: parent.width
                    height: 50

                    Row {
                        spacing: 10
                        Text {
                            id: station_name
                            text: {(stationModel.count > 0) ? stationModel.get(0).name : ""}
                            font.pointSize: 14

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    current_station++;
                                    if (current_station >= stationModel.count)
                                        current_station = 0;
                                    station_name.text = stationModel.get(current_station).name;
                                    xmlModel.query = "/data/station[@id='"+stationModel.get(current_station).id+"']/item[not(@current)]";
                                    xmlModel.reload();
                                    currentxmlModel.query = "/data/station[@id='"+stationModel.get(current_station).id+"']/item[@current='true']";
                                    currentxmlModel.reload();
                                }
                            }
                        }
                        Text {
                            text: "Refresh"
                            font.pointSize: 14
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    //appsModel.launch("/opt/com.meecast.omweather/bin/xml-qml")
                                    console.log("launch");
                                    stationModel.reload();
                                    xmlModel.reload();
                                    currentxmlModel.reload();
                                }
                            }
                        }
                    }

                }

                ListView {
                    id: currentlist
                    model: currentxmlModel
                    delegate: itemDelegateCurrent
                    //anchors.top: parent.top
                    //anchors.left: parent.left
                    width: parent.width
                    height: 200
                }
                GridView {
                    id: list

                    //anchors.top: currentlist.bottom
                    cellWidth: parent.width/2; cellHeight: 120
                    model: xmlModel
                    delegate: itemDelegate
                    width: parent.width
                    height: parent.height - 200
                    //anchors.fill: parent
                }
            }


}


