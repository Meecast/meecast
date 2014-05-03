import QtQuick 2.0
import Sailfish.Silica 1.0
Component {
   id: weatherStationDelegate

    Rectangle {
        id: main_rectangle
        width: main.screen_width 
        height: main.screen_height
        property int icon_size: 128
        property int main_index : index
        color: Config.transparency ? "transparent" : "black"
        SilicaFlickable {
            id: flickable
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: toolbar.top
            pressDelay: 0
            contentWidth: flickable.width
            contentHeight: station_rect.height + current_rect.height + list.height
            flickableDirection: Flickable.VerticalFlick

            PageHeader {
                id: stationname
                title: main.stationname1_index(index) 
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("111111111111111")
                    }
                }
            }
            PullDownMenu {
                MenuItem {
                     id: item4menu
                     text: Config.tr("About")
                     onClicked: {
                          openFile("AboutPage.qml");
                     }
                }
                MenuItem {
                    id: item1menu
                    text: Config.tr("Settings")
                    onClicked: {
                         openFile("SettingsPage.qml");
                    }
                }
                MenuItem {
                    id: item2menu
                    text: Config.tr("Refresh")
                    onClicked: {
                        if (!isUpdate) main.update();
                    }
                }
		/*
                MenuItem {
                    id: item3menu
                    visible: menuitemgps
                    text: Config.tr("Adjust GPS station")
                    onClicked: {
                        Config.updategpsstation();
                    }
	        }
                */
            }

            Rectangle {
                id: startview
                visible: Config.stationname == "Unknown" ? true : false
                width: parent.width
                height: main.screen_height - 72 - 36
                Rectangle {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    height: 72
                    color: Config.transparency ? "transparent" : "black"
                }
                Text {
                    id: empty_text
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    height: 72
                    color: "white"
                    font.pointSize: 28
                    text: Config.tr("MeeCast")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Loader {
                    id: empty_background1
                    anchors.top: empty_text.bottom
                    anchors.left: parent.left
                    width: parent.width
                    height: 274
                    sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
                }
                Rectangle {
                    anchors.top: empty_background1.bottom
                    width: parent.width
                    height: dataview.height - 274
                    color: Config.transparency ? "transparent" : "black"
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: Config.tr("No locations are set up yet.")
                    font.pixelSize: 54 
                    color: "#999999"
                    wrapMode: Text.Wrap
                    width: parent.width - 2*margin
                    //anchors.verticalCenter: parent.verticalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 250
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: margin
                    anchors.rightMargin: margin
                }
                Button {
                    text: Config.tr("Set locations")
                    onClicked: {
                        main.openFile("SourcePage.qml")
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                }
            }

            Item {
                id: mainview
                visible: Config.stationname == "Unknown" ? false : true
                anchors.fill: parent
                Rectangle {
                    id: updateview
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    height: 92
                    color: Config.transparency ? "transparent" : "black"
                    visible: isUpdate ? true : false

                    Column {
                        anchors.fill: parent
                        spacing: 8
                        Label {
                            text: Config.tr("Updating forecast")
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        ProgressBar {
                            id: pb
                            indeterminate: true
                            width: parent.width-2*margin
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }
                Rectangle {
                    id: station_rect
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    height: 92
                    //color: Config.transparency ? "transparent" : "black"
                    color: "transparent"
                    visible: isUpdate ? false : true
                }
                Rectangle {
                    id: dataview
                    visible: (Forecast_model.rowCount() == 0 && Current.rowCount() == 0) ? true : false
                    anchors.top: station_rect.bottom
                    width: parent.width
                    //height: current_rect.height + list.height
                    //height: main.screen_height - 72 - 72 - 36
                    height: main.screen_height - 92 
                    color: Config.transparency ? "transparent" : "black"
                    Loader {
                        id: empty_background
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: 274
                        sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
                    }
                    Rectangle {
                        anchors.top: empty_background.bottom
                        width: parent.width
                        height: dataview.height - 274
                        color: Config.transparency ? "transparent" : "black"
                    }
                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: Config.tr("Looks like there's no info for this location.")
                        font.pixelSize: 54 
                        color: "#999999"
                        wrapMode: Text.Wrap
                        width: parent.width - 2*margin
                        //anchors.verticalCenter: parent.verticalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 200
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: margin
                        anchors.rightMargin: margin
                    }
                    Button {
                        text: Config.tr("Try to update")
                        onClicked: {
                            main.update();
                            dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
                            current_rect.visible = Current.rowCount() == 0 ? false : true;
                            list.visible = (Forecast_model.rowCount() == 0) ? false : true;
                            

                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 150
                    }

                }
                Rectangle {
                    id: current_rect
                    visible: Current.rowCount() == 0 ? false : true
                    anchors.top: station_rect.bottom
                    width: parent.width
                    height: 274
                    //color: getColor(Current.temperature_high)
                    Loader {
                        anchors.fill: parent
                        sourceComponent: Image {source: Config.imagespath + "/mask_background_main.png"}
                    }
                    ListView {
                        anchors.fill: parent
                        model: current_stub
                        delegate: currentDelegate
                        interactive: false
                    }
                    Component {
                        id: currentDelegate
                        Item {
                            Text {
                                id: now
                                width: main.screen_width/2 - icon_size/2 
                                height: 84
                                anchors.top: parent.top
                                anchors.left: parent.left
                                color: "white"
                                text: main.current_model("current", main_index) == true ? Config.tr("Now") : Config.tr("Today")
                                font.pointSize: 26
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                            }
                            Image {
                                id: icon
                                source: Config.iconspath + "/" + Config.iconset + "/" + main.current_model("pict", main_index) 
                                width: icon_size 
                                height: icon_size
                                anchors.top: parent.top
                                anchors.topMargin: -22
                                anchors.left: now.right
                                smooth: true    
                            }
                            Text {
                                anchors.top: parent.top
                                anchors.left: icon.right
                                id: temp_text
                                width: main.screen_width/2 - icon_size/2
                                height: 84
                                color: "white"
                                text: model.temp + '°'
                                font.pointSize: 26
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                Component.onCompleted: {
                                    if (main.current_model("temp", main_index) == "N/A"){
                                        temp_text.text = ""
                                        if (main.current_model("temp_high", main_index) != "N/A")
                                           temp_text.text =  main.current_model("temp_high", main_index) + '°'
                                        if ((main.current_model("temp_low", main_index)  != "N/A") && (main.current_model("temp_high", main_index) != "N/A"))
                                           temp_text.text =  temp_text.text + " / "
                                        if (main.current_model("temp_low", main_index)  != "N/A")
                                           temp_text.text = temp_text.text + main.current_model("temp_low", main_index) + '°'
                                        if (Config.transparency)
                                            current_rect.color = "transparent"
					else
                                            /* current_rect.color = getColor(main.current_model("temp_high", main_index));
                                            not sure what to do here, with the coloring it looks aweful on black theme */
                                            current_rect.color = "transparent"
                                    }else{
                                       temp_text.text = main.current_model("temp", main_index) + '°'
                                       if (Config.transparency)
                                           current_rect.color = "transparent"
				       else
                                           /* current_rect.color = getColor(main.current_model("temp", main_index));
                                           not sure what to do here, with the coloring it looks aweful on black theme */
                                           current_rect.color = "transparent"
                                    }
                                }
                            }
                            Rectangle {
                               id: desc  
                               height: 44
                               color: Config.transparency ? "transparent" : "black"
                               width: current_rect.width 
                               anchors.left: parent.left
                               anchors.top: now.bottom
                               property color textColor: "white"
                               Row {  
                                   id: desc_row
                                   width: parent.width 
                                   Text { 
                                     id: text; 
                                     font.pointSize: 20; 
                                     width: parent.width 
                                     color: desc.textColor; 
                                     text: main.current_model("description", main_index); 
                                     verticalAlignment: Text.AlignVCenter; 
                                     horizontalAlignment: (main.current_model("description", main_index) >35) ? Text.AlignHLeft : Text.AlignHCenter; 
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: {
                                                if (text_anim.running){
                                                    text_anim.running = false;
                                                }else{
                                                    text_anim.running = true;
                                                }
                                            }
                                        }
                                    }  
                                    NumberAnimation on x { 
                                        id: text_anim; 
                                        from: 450; to: -500 ; 
                                        duration: 10000; 
                                        loops: Animation.Infinite; 
                                        running : (main.current_model("description", main_index) >35) ? true : false;  
                                    }  
                                }  
                            }
                            Image {
                                id: humidity
                                source: Config.imagespath + "/humidity.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: 32
                                anchors.left: parent.left
                                anchors.leftMargin: margin
                                width: 30
                                height: 30
                                smooth: true
                            }
                            Text {
                                text: main.current_model("humidity", main_index) + "%"
                                anchors.left: humidity.right
                                anchors.leftMargin: 8
                                anchors.top: desc.bottom
                                anchors.topMargin: 32
                                height: 30
                                color: "white"
                                font.pointSize: 18
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                id: wind_direction_background
                                //source: Config.imagespath + "/wind_direction.png"
                                source: Config.imagespath + "/wind_direction_background.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: 32
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2
                                width: 30
                                height: 30
                                smooth: true
                            }
                            Image {
                                id: wind_direction
                                //source: Config.imagespath + "/wind_direction.png"
                                source: Config.imagespath + "/wind_direction_arrow.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: 32
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2
                                width: 30
                                height: 30
                                smooth: true
                                transform: Rotation {
                                    origin.x: 15
                                    origin.y: 15
                                    angle: main.current_model("wind_direction", main_index) == "N/A" ? 0 : main.getAngle(main.current_model("wind_direction", main_index))
                                }
                            }
                            Text {
                                text: Config.tr(main.current_model("wind_direction", main_index))
                                anchors.left: wind_direction.right
                                anchors.leftMargin: 8
                                anchors.top: desc.bottom
                                anchors.topMargin: 32
                                height: 30
                                color: "white"
                                font.pointSize: 18
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                    id: pressure
                                    source: Config.imagespath + "/pressure.png"
                                    anchors.top: humidity.bottom
                                    anchors.topMargin: 22
                                    anchors.left: parent.left
                                    anchors.leftMargin: margin
                                    width: 30
                                    height: 30
                                    smooth: true
                            }
                            Text {
                                text: main.current_model("pressure", main_index) + ' ' + Config.tr(Config.pressureunit)
                                anchors.left: pressure.right
                                anchors.leftMargin: 8
                                anchors.top: humidity.bottom
                                anchors.topMargin: 22
                                height: 30
                                color: "white"
                                font.pointSize: 18
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                id: wind_speed
                                source: Config.imagespath + "/wind_speed.png"
                                anchors.top: humidity.bottom
                                anchors.topMargin: 22
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2 
                                width: 30
                                height: 30
                                smooth: true
                            }
                            Text {
                                text: (Config.windspeedunit == "Beaufort scale") ? main.current_model("wind_speed", main_index) : main.current_model("wind_speed", main_index) + ' ' + Config.tr(Config.windspeedunit)
                                anchors.left: wind_speed.right
                                anchors.leftMargin: 8
                                anchors.top: humidity.bottom
                                anchors.topMargin: 22
                                height: 30
                                color: "white"
                                font.pointSize: 18
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("current day onclicked");
                            Config._current_station_id(main.real_current_id)
                            //Config._current_station_id(0)
                            console.log(Config._current_station_id())
                            main.updatemodels()

                            Current_night.update_model(1);
                            Forecast_night_model.update_model(3);
                            Forecast_hours_model.update_model(4);

                            pageStack.push(Qt.resolvedUrl("FullWeatherPage.qml"),
                                           {day: 0, day_period: "day", current: true }
                                          )
                        }
                    }
                }
                ListView {
                    id: list
                    visible: Forecast_model.rowCount() == 0 ? false : true
                    anchors.top: current_rect.bottom
                    model:  forecast_stub
                    delegate: itemDelegate
                    width: parent.width
 //                   height: 80 *  main.get_count_days(index)
                    height: 80 * 21 
                    interactive: false
                    clip: true
                    Component.onCompleted: {
                       console.log("list onCompleted ", main_index, forecast_stub.count)
                       list.height = 80 * Forecast_model.rowCount() 
                    }
                }
                Component {
                    id: itemDelegate
                    Item {
                        id: day
                        width: parent.width
                        height: 80
                        ListView.onAdd: {
		            /* Hack */		
                            list.height = 80 * Forecast_model.rowCount() 
			    forecast_stub.remove(list.count-1)
                        }		    
                        visible: main.forecast_model(index, "fulldate", main_index) == undefined ? false : true

                        Rectangle {
                            width: parent.width
                            height: 80
                            color: Config.transparency ? ((index % 2 != 0) ? "transparent" : "#10ffffff") : ((index % 2 != 0) ? "black" : "#0f0f0f")
                            Text {
                                id: txt_date
                                text: main.forecast_model(index, "fulldate", main_index) == undefined ? "" : main.forecast_model(index, "fulldate", main_index)
                                color: "#889397"
                                font.pointSize: 18
                                anchors.left: parent.left
                                anchors.leftMargin: margin
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                text: main.forecast_model(index, "shortdate", main_index) == undefined ? "" : main.forecast_model(index, "shortdate", main_index) 
                                color: "white"
                                font.pointSize: 18
                                anchors.left: parent.left
                                anchors.leftMargin: (margin + txt_date.width + 8)
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                source: main.forecast_model(index, "pict", main_index) == undefined ? Config.iconspath + "/" + Config.iconset + "/49.png" : Config.iconspath + "/" + Config.iconset + "/" +  main.forecast_model(index, "pict", main_index)
                                width: 64
                                height: 64
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                smooth: true
                            }
                            Text {
                                id: txt_temphi
                                font.pointSize: 18
                                color: main.forecast_model(index, "temp_high", main_index) != undefined ? getColor(main.forecast_model(index, "temp_high", main_index)) : getColor(0)
                                text: main.forecast_model(index, "temp_high", main_index) + '°'
                                anchors.right: parent.right
                                anchors.rightMargin: margin + 70
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: txt_templo
                                font.pointSize: 18
                                color: "#889397"
                                text: main.forecast_model(index, "temp_low", main_index) + '°'
                                anchors.right: parent.right
                                anchors.rightMargin: margin
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log("day onclicked");

                                    Current_night.update_model(1);
                                    Forecast_night_model.update_model(3);
                                    Forecast_hours_model.update_model(4);

                                    pageStack.push(Qt.resolvedUrl("FullWeatherPage.qml"),
                                                   {day: index, day_period: "day" }
                                                   )
                                }
                            }

                        }
                    }
                } //component itemDelegate
            }
        }

        Rectangle {
            id: toolbar
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 90
            color: Config.transparency ? Theme.secondaryHighlightColor : "black"
            opacity: 0.8
            Image {
                id: sourceicon
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source_index(index) + ".png"
                smooth: true

                Component.onCompleted: {
//                    console.log("Index")
//                    console.log(index)
//                    console.log( Config.source_index(index))
//                    console.log( Config.stationname)
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        Config.showweb();	
                    }
                }
            }
        }

    }
}

