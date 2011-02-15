import Qt 4.7
/*
LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"
*/
Rectangle {
    width: 800
    height: 480

    Rectangle {
        id: panel
        z: 5
        width: parent.width
        height: 100
        anchors.top: parent.top
        anchors.left: parent.left

        TextButton {
            text: Config.stationname
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            onClicked: {
                Config.changestation();
                list.model.update(Config.filename);
                text = Config.stationname;
            }
        }

        ImageButton {
            id: refresh
            imagefile: Config.iconsbutton + "/" + "refresh.png"
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            onClicked: {
                console.log("refresh");
                Config.updatestations();
                list.model.update(Config.filename);
            }
        }
        ImageButton {
            id: config
            imagefile: Config.iconsbutton + "/" + "config.png"
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: refresh.left
            anchors.rightMargin: 10
        }
        ImageButton {
            id: about
            imagefile: Config.iconsbutton + "/" + "about.png"
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.right: config.left
            anchors.rightMargin: 10
            onClicked: {
                console.log("about");
                list.opacity = 0;
                aboutblock.opacity = 1;
            }
        }
    }

    Component {
        id: itemDelegate

        Item {
            id: day
            property  real detailsOpacity: 0

            width: 160
            height: 190

            Rectangle {
                id: background
                x: 2; y:2; width: parent.width - x*2; height: parent.height - y*2
                //color: "green"
            }

            MouseArea {
                id: toDetail
                anchors.fill: parent
                onClicked: if (day.detailsOpacity == 0) day.state = 'Details'; else day.state = '';

            }


            Text {
                id: day_name
                text: shortdate
                color: Config.fontcolor
                anchors.left: parent.left
                anchors.leftMargin: 20
                //anchors.centerIn: parent
                //anchors.top: parent.top
            }

            Image {
                id: forecast_icon
                source: Config.iconspath + "/" + Config.iconset + "/" + pict
                width: 64
                height: 64
                anchors.top: day_name.bottom
                //anchors.topMargin: 20
                //anchors.leftMargin: 5
                anchors.left: parent.left
            }
            Text {
                id: temperature
                text: ((temp_low != "N/A") ? (temp_low + '°' +Config.temperatureunit + "\n") : "") +
                        ((temp_high != "N/A") ? (temp_high + '°' +Config.temperatureunit) : "")
                anchors.top: forecast_icon.top; anchors.topMargin: 10
                anchors.left: forecast_icon.right; anchors.leftMargin: 10
                opacity: 1 - day.detailsOpacity
            }
            Item {
                id: details
                anchors {top: forecast_icon.top; topMargin: 50; bottom: parent.bottom; bottomMargin: 10; left: forecast_icon.right}
                opacity: day.detailsOpacity

                Column {
                    spacing: 5

                    Text {
                        text: description
                        font.pointSize: 12
                    }
                    Text {
                        text: "Temperature: " +
                                ((temp_low != "N/A") ? (temp_low + '°' +Config.temperatureunit + " .. ") : "") +
                                ((temp_high != "N/A") ? (temp_high + '°' +Config.temperatureunit) : "")
                        font.pointSize: 12
                    }
                    Text {
                        text: (humidity != "N/A") ? ("Humidity: " + humidity + "%") : ""
                        font.pointSize: 12
                    }
                    Text {
                        text: (pressure != "N/A") ? ("Pressure: " + pressure) : ""
                        font.pointSize: 12
                    }
                    Text {
                        text: (wind_direction != "N/A") ? ("Wind: " + wind_direction) : ""
                        font.pointSize: 12
                    }
                    Text {
                        text: (wind_speed != "N/A") ? ("Wind speed: " + wind_speed + "m/s") : ""
                        font.pointSize: 12
                    }
                }


            }

            states: State {
                name: "Details"

                PropertyChanges {
                    target: day_name
                    font.pointSize: 16
                    text: fulldate
                    anchors.leftMargin: 50
                }

                PropertyChanges {
                    target: forecast_icon
                    width: 256
                    height: 256
                }
                PropertyChanges {
                    target: day
                    detailsOpacity: 1
                    x: 0
                }
                PropertyChanges {
                    target: day
                    z: 100
                    width: list.width
                    height: list.height
                }

                PropertyChanges {
                    target: day.GridView.view
                    explicit: true
                    contentY: day.y

                }
                PropertyChanges {
                    target: day.GridView.view
                    interactive: false
                }
            }

        }

    }

    GridView {
        id: list
        z: 1
        //anchors.fill: parent
        anchors.top: panel.bottom
        width: parent.width
        height: parent.height - panel.height
        cellWidth: 160; cellHeight: 190
        model: Forecast_model
        delegate: itemDelegate
    }
    Item {
        id: aboutblock
        z: 10
        anchors.top: panel.bottom
        width: parent.width
        height: parent.height - panel.height
        opacity: 0
        Row {
            spacing: 20
            anchors.centerIn: parent
            Image {
                id: about_icon
                source: Config.iconspath + "/" + Config.iconset + "/21.png"
                width: 256
                height: 256
                //anchors.top: day_name.bottom
                //anchors.topMargin: 20
                //anchors.leftMargin: 5
                //anchors.left: parent.left
            }
            Text {
                font.pointSize: 14
                text: "Weather forecast for the Meego. Version 0.3.\n" +
                  "Author and maintenance: Vlad Vasiliev, <vlad@gas.by>\n" +
                  "Maintenance: Pavel Fialko, <pavelnf@gmail.com>\n          Tanya Makova, <tanyshk@gmail.com>\n" +
                  "Design UI and default iconset:\n           Andrew Zhilin, <az@pocketpcrussia.com>\n"
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.opacity = 0;
                list.opacity = 1;
            }

        }
    }

}
