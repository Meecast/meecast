import QtQuick 2.0
import Sailfish.Silica 1.0
Component {
   id: weatherStationDelegate

    Rectangle {
        width: screen_width
        height: main.screen_height 
        color: "black"
    //PageHeader {
    //    id: stationname
    //    title: Config.stationname
    //}

//    tools: ToolBarLayout {
//        ToolIcon {
//            iconId: (!isUpdate) ? "toolbar-refresh" : "toolbar-stop-dimmed"
//            onClicked: {
//                if (!isUpdate) main.update();
//                //console.log(main.getColor(-15));
//            }
//        }
//        Image {
//            id: sourceicon
//	    anchors.horizontalCenter: parent.horizontalCenter
//            source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
//            smooth: true
//	    MouseArea{
//	        anchors.fill: parent
//		onClicked: {
// 		    Config.showweb();	
//		}
//	    }
//        }
//        ToolIcon {
//            iconId: "toolbar-view-menu"
//            onClicked: {(myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()}
//            anchors.right: parent == undefined ? undefined : parent.right
//        }
//    }
//    orientationLock: PageOrientation.LockPortrait
    SilicaFlickable {
        id: flickable
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top
        PageHeader {
            id: stationname
            title: main.stationname1_index(index) 
        }
        PullDownMenu {
            MenuItem {
                id: item1menu
                text: Config.tr("Settings")
                onClicked: {
                     openFile("SettingsPage.qml");
                }
            }
            MenuItem {
                id: item2menu
                visible: menuitemgps
                text: Config.tr("Adjust GPS station")
                onClicked: {
                    Config.updategpsstation();
                }
           }
           MenuItem {
                id: item3menu
                text: Config.tr("About")
                onClicked: {
                     openFile("AboutPage.qml");
                }
           }

        }

        contentWidth: flickable.width
        contentHeight: station_rect.height + current_rect.height + list.height
        flickableDirection: Flickable.VerticalFlick
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
                color: "black"
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
                color: "black"
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

            /*
            Loader {
                id: background
                anchors.fill: parent
                sourceComponent: Image {source: "../core/data/images/background.png"}
            }*/
            Rectangle {
                id: updateview
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: 72
                color: "black"
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
                color: "transparent"
                //color: "black"
                visible: isUpdate ? false : true
/*
                Text {
                    id: stationname
                    anchors.top: parent.top
                    anchors.left: left_arrow.right
                    //anchors.right: right_arrow.left
                    height: parent.height
                    width: parent.width - right_arrow.width - left_arrow.width
                    text: Config.stationname
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    font.pointSize: 20
                    //wrapMode: Text.Wrap
                    elide: Text.ElideRight
                }
                */
               /*
                Rectangle {
                    id: left_arrow
                    width: 72
                    height: 72
                    anchors.top: parent.top
                    anchors.left: parent.left
                    color: "black"
                    visible: Config.prevstationname == "" ? false : true;
                    Image {
                        id: prevstationimage
                        source: Config.imagespath + "/arrow_left.png"
                        width:  62 
                        height: 62
                        anchors.top: parent.top
                        anchors.left: parent.left
                        //anchors.leftMargin: margin
                        smooth: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (prevstationimage.visible){
                                Config.prevstation();
                                main.updatestationname();
                            }
                        }
                    }

                }
                */
                /*
               Rectangle {
                    id: right_arrow
                    width: 72
                    height: 72
                    anchors.top: parent.top
                    anchors.right: parent.right
                    color: "black"
                    visible: Config.nextstationname == "" ? false : true;
                    Image {
                        id: nextstationimage
                        source: Config.imagespath + "/arrow_right.png"
                        width: 62 
                        height: 62
                        anchors.top: parent.top
                        anchors.right: parent.right
                        //anchors.verticalCenter: parent.verticalCenter
                        //anchors.rightMargin: margin
                        smooth: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (nextstationimage.visible){
                                Config.nextstation();
                                main.updatestationname();
                            }
                        }
                    }
                }
                */

 
                /*
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_title.png"}
                }*/
            }
            Rectangle {
                id: dataview
                visible: (Forecast_model.rowCount() == 0 && Current.rowCount() == 0) ? true : false
                anchors.top: station_rect.bottom
                width: parent.width
                //height: current_rect.height + list.height
                //height: main.screen_height - 72 - 72 - 36
                height: main.screen_height - 72 
                //color: "black"
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
                    color: "black"
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
                    model: Current
                    delegate: currentDelegate
                    interactive: false
                }
                Component {
                    id: currentDelegate
                    Item {
                        //anchors.fill: ListView

                        Text {
                            id: now
                            width: 160
                            height: 84
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: margin
                            color: "white"
                            text: model.current == true ? Config.tr("Now") : Config.tr("Today")
                            font.pointSize: 26
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter

                        }
                        Image {
                            id: icon
                            source: Config.iconspath + "/" + Config.iconset + "/" + model.pict
                            width: 128
                            height: 128
                            anchors.top: parent.top
                            anchors.topMargin: -22
                            anchors.left: now.right
                            smooth: true    
                        }
                        Text {
                            anchors.top: parent.top
                            anchors.left: icon.right
                            anchors.rightMargin: margin
                            id: temp_text
                            width: 160
                            height: 84
                            color: "white"
                            text: model.temp + '°'
                            font.pointSize: 26
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Component.onCompleted: {
                                if (model.temp == "N/A"){
				                    temp_text.text = ""
				                    if (model.temp_high != "N/A")
                                       temp_text.text = model.temp_high + '°'
				                    if ((model.temp_low != "N/A") && (model.temp_high != "N/A"))
				                       temp_text.text =  temp_text.text + " / "
				                    if (model.temp_low != "N/A")
                                       temp_text.text = temp_text.text + model.temp_low + '°'
                                    current_rect.color = getColor(model.temp_high);
                                }else{
                                   temp_text.text = model.temp + '°'
                                   current_rect.color = getColor(model.temp);
				                }
                            }
                        }
		        Rectangle {
			       id: desc  
			       height: 44
			       color: "transparent"
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
					  text: model.description; 
					  verticalAlignment: Text.AlignVCenter; 
					  horizontalAlignment: (model.description.length >35) ? Text.AlignHLeft : Text.AlignHCenter; 
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
					 running : (model.description.length >35) ? true : false;  
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
                            text: model.humidity+"%"
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
                            anchors.leftMargin: margin+224
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
                            anchors.leftMargin: margin+224
                            width: 30
                            height: 30
                            smooth: true
                            transform: Rotation {
                                origin.x: 15
                                origin.y: 15
                                angle: main.getAngle(model.wind_direction)
                            }
                        }
                        Text {
                            text: Config.tr(model.wind_direction)
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
                            text: model.pressure + ' ' + Config.tr(Config.pressureunit)
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
                            anchors.leftMargin: margin+224
                            width: 30
                            height: 30
			    smooth: true
                        }
                        Text {
                            text: (Config.windspeedunit == "Beaufort scale") ? model.wind_speed : model.wind_speed + ' ' + Config.tr(Config.windspeedunit)
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
                model: Forecast_model
                delegate: itemDelegate
                width: parent.width
                height: 80 * Forecast_model.rowCount()
                //height: 800
                interactive: false
                clip: true

            }
            Component {
                id: itemDelegate
                Item {
                    id: day
                    width: parent.width
                    height: 80

                    Rectangle {
                        width: parent.width
                        height: 80
                        color: (index % 2 != 0) ? "black" : "#0f0f0f"

                        Text {
                            id: txt_date
                            text: model.fulldate
                            color: "#889397"
                            font.pointSize: 18
                            anchors.left: parent.left
                            anchors.leftMargin: margin
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: model.shortdate
                            color: "white"
                            font.pointSize: 18
                            anchors.left: parent.left
                            anchors.leftMargin: (margin + txt_date.width + 8)
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Image {
                            source: Config.iconspath + "/" + Config.iconset + "/" + model.pict
                            width: 64
                            height: 64
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            smooth: true
                        }
                        Text {
                            id: txt_temphi
                            font.pointSize: 18
                            color: getColor(temp_high)
                            text: model.temp_high + '°'
                            anchors.right: parent.right
                            anchors.rightMargin: margin + 70
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            id: txt_templo
                            font.pointSize: 18
                            color: "#889397"
                            text: model.temp_low + '°'
                            anchors.right: parent.right
                            anchors.rightMargin: margin
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }


                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("day onclicked");
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
        height: 72
        color: "black"
        Image {
            id: refresh_button
            anchors.left: parent.left
            source:  "image://theme/icon-m-refresh"
            visible: isUpdate ? false : true
	        MouseArea{
	            anchors.fill: parent
		        onClicked: {
                    if (!isUpdate) main.update();
		        }
            }
        } 
        BusyIndicator {
            running: isUpdate ? true : false
            anchors.left: parent.left
            visible: isUpdate ? true : false
            size: BusyIndicatorSize.Medium
            anchors.verticalCenter: parent.verticalCenter
        }
        Image {
            id: sourceicon
	        anchors.horizontalCenter: parent.horizontalCenter
	        anchors.verticalCenter: parent.verticalCenter
            source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source() + ".png"
            smooth: true
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

