import Qt 4.7

Rectangle {
    id: rect
    width: 500
    height: 900

    property int current_station: 0;
    // change station and update button
    Rectangle {
        id: rect1
        width: parent.width
        height: 62
        color: "#202020"
        anchors.top: parent.top

        Text {
            id: station_name
            font.pointSize: 24
            color: "white"
            text: {(stationModel.count > 0) ? stationModel.get(0).name : ""}
            anchors.leftMargin: 10
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

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
        Image {
            //source: "/opt/com.meecast.omweather/share/buttons_icons/refresh_arrows.png"
            source: "refresh_arrows.png"
            anchors.rightMargin: 10
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

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
    // current weather
    Rectangle {
        id: rect2
        anchors.top: rect1.bottom
        width: parent.width
        height: 200
        gradient: Gradient {
            GradientStop {position: 0.0; color: "#000000"}
            GradientStop {position: 1.0; color: "#282828"}
        }
        ListView {
            id: currentlist
            model: currentxmlModel
            delegate: itemDelegateCurrent
            width: parent.width
            height: parent.height
            boundsBehavior: "StopAtBounds"
        }
        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: "#161616"
        }
    }
    // list view with forecast
    ListView {
        id: list
        anchors.top: rect2.bottom
        model: xmlModel
        delegate: itemDelegate
        width: parent.width
        height: parent.height - rect1.height - rect2.height
        //boundsBehavior: "StopAtBounds"
    }


            // model with stantions
            XmlListModel {
                id: stationModel
                source: "../test/qmldata.xml"
                query: "/data/station"

                XmlRole {name:  "id"; query: "@id/string()"}
                XmlRole {name:  "name"; query: "@name/string()"}

            }
            // model for current weather
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
            // model for forecast
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
            // delegate for current list view
            Component {
                id: itemDelegateCurrent
                Item {
                    //anchors.fill: parent
                    width: parent.width
                    height: parent.height

                    Text {
                        id: txt_dayname
                        font.pointSize: 18
                        color: "white"
                        anchors.topMargin: 20
                        anchors.leftMargin:10
                        anchors.top: parent.top
                        anchors.left: parent.left
                        text: dayname
                    }

                    Text {
                        id: txt_desc
                        font.pointSize: 14
                        color: "white"
                        anchors.topMargin: 20
                        anchors.leftMargin:10
                        anchors.top: txt_dayname.bottom
                        anchors.left: parent.left
                        text: description
                    }
                    Text {
                        id: txt_humidity
                        font.pointSize: 14
                        color: "white"
                        anchors.leftMargin:10
                        anchors.top: txt_desc.bottom
                        anchors.left: parent.left
                        text: qsTr("Humidity") + ": " + humidity
                    }
                    Text {
                        id: txt_wind
                        font.pointSize: 14
                        color: "white"
                        anchors.leftMargin:10
                        anchors.top: txt_humidity.bottom
                        anchors.left: parent.left
                        text: qsTr("Wind") + ": " + wind_direction
                    }
                    Text {
                        id: txt_speed
                        font.pointSize: 14
                        color: "white"
                        anchors.leftMargin:10
                        anchors.top: txt_wind.bottom
                        anchors.left: parent.left
                        text: qsTr("Speed") + ": " + wind_speed
                    }
                    Component {
                        id: single_temp
                        Text {
                            id: txt_temp
                            anchors.rightMargin: 30
                            anchors.right: parent.right
                            font.pointSize: 18
                            color: "white"
                            text: temperature
                        }
                    }
                    Component {
                        id: multi_temp
                        Item {
                            anchors.rightMargin: 60
                            anchors.right: parent.right
                            Text {
                                id: txt_templo
                                font.pointSize: 18
                                color: "#889397"
                                text: temperature_low
                            }
                            Text {
                                id: txt_temphi
                                anchors.rightMargin: 20
                                anchors.right: txt_templo.left
                                font.pointSize: 18
                                color: "#31c2ee"
                                text: temperature_high
                            }
                        }
                    }
                    Loader {
                        id: temp
                        anchors.topMargin: 20
                        anchors.rightMargin:10
                        anchors.top: parent.top
                        anchors.right: parent.right
                        sourceComponent: temperature ? single_temp : multi_temp
                    }
                    Image {
                        source: icon
                        anchors.bottomMargin: 20
                        anchors.rightMargin:10
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                    }


                }
            }
            // delegate for forecast list view
            Component {
                id: itemDelegate
                Item {
                    id: day
                    width: parent.width
                    height: 80

                    Rectangle {
                        width: parent.width
                        height: 80
                        color: (id_item % 2 != 0) ? "#202020" : "#282828"

                        Rectangle {
                            anchors.top: parent.top
                            width: parent.width
                            height: 1
                            color: (id_item % 2 != 0) ? "#373737" : "#3d3d3d"
                        }
                        Rectangle {
                            anchors.bottom: parent.bottom
                            width: parent.width
                            height: 1
                            color: (id_item % 2 != 0) ? "#161616": "#1c1c1c"
                        }

                        Text {
                            text: dayname
                            color: "white"
                            font.pointSize: 16
                            anchors.leftMargin: 10
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Image {
                            source: icon
                            width: 64
                            height: 64
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Component {
                            id: single_temp
                            Item {
                                anchors.right: parent.right
                                height: parent.height
                                Text {
                                    id: txt_temp
                                    anchors.rightMargin: 30
                                    anchors.right: parent.right
                                    font.pointSize: 16
                                    color: "white"
                                    text: temperature
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }
                        Component {
                            id: multi_temp
                            Item {
                                anchors.rightMargin: 60
                                anchors.right: parent.right
                                height: parent.height
                                Text {
                                    id: txt_templo
                                    font.pointSize: 16
                                    color: "#889397"
                                    text: temperature_low
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Text {
                                    id: txt_temphi
                                    anchors.rightMargin: 20
                                    anchors.right: txt_templo.left
                                    font.pointSize: 16
                                    color: "#31c2ee"
                                    text: temperature_high
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                            }
                        }

                        Loader {
                            id: temp
                            //anchors.verticalCenter: parent.verticalCenter
                            anchors.rightMargin:10
                            anchors.right: parent.right
                            height: parent.height
                            sourceComponent: (temperature_high) ? multi_temp : single_temp
                        }
                        /*
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

                        }*/

                    }



                }

            }

        }
