import QtQuick 2.0
import "./"
//import Sailfish.Silica 1.0
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
Component {
   id: weatherStationDelegate


    Rectangle {
        id: main_rectangle
        width: main.screen_width 
        height: main.screen_height
        //height: main.screen_height - headerRowLayout.height
        //property real ratio: main.screen_width/540
        //property real ratio: Theme.pixelRatio
        property real ratio: Screen.devicePixelRatio
        property int icon_size: 128*ratio
        property int top_rect_height: 274*ratio
        property int row_rect_height: 80*ratio
        property int small_box_rect_height: 30*ratio
        property int small_box_rect_top_margin: 32*ratio
        property int small_box_rect_top_margin2: 22*ratio
        property int small_box_rect_left_margin: 8*ratio
        property int current_text_rect_height: 84*ratio
        property int description_text_rect_height: 44*ratio
        property int description_FontPointSize: 20*ratio
        property int medium_FontPointSize: 18*ratio
        property int large_FontPointSize: 26*ratio
        property int huge_PixelSize: 54*ratio
        property int main_index : index
        property bool flipmoving : flipmoving 
        color: Config.transparency ? "transparent" : "black"
        /*
        GlassItem {
            id: left_indicator
            visible: (main_index > 0 && flipmoving == false) ? true : false
            color: Theme.highlightColor
            falloffRadius: 0.05
            radius: 0.05
            height: parent.height / 2
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.left
            Behavior on falloffRadius {
                NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }
            }
            brightness: 1.0
            Behavior on brightness {
                NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }
            }
        }*/

        Flickable {
            id: flickable
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: toolbar.top
            pressDelay: 0
            contentWidth: flickable.width
            contentHeight: station_rect.height + current_rect.height + list.height
            flickableDirection: Flickable.VerticalFlick

            /*
            
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
            */
            /*
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
            }
            */

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
                    color: Theme.primaryColor
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
                    height: top_rect_height 
                    sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
                }
                Rectangle {
                    anchors.top: empty_background1.bottom
                    width: parent.width
                    height: dataview.height - top_rect_height 
                    color: Config.transparency ? "transparent" : "black"
                }
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: Config.tr("No locations are set up yet.")
                    font.pixelSize: huge_PixelSize 
                    color: Theme.secondaryColor
                    wrapMode: Text.Wrap
                    width: parent.width - 2*margin
                    //anchors.verticalCenter: parent.verticalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 250*ratio
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
                    anchors.bottomMargin: 50*ratio
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
                    height: 92*ratio
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
                    height: screen_height*0.1 
                    //color: Config.transparency ? "transparent" : "black"
                    color: "transparent"
                    visible: isUpdate ? false : true
                }
                Rectangle {
                    id: dataview
                    visible: (forecast_model_count(main_index) == 0 && current_model_count(main_index) == 0) ? true : false
                    anchors.top: station_rect.bottom
                    width: parent.width
                    height: main.screen_height - updateview.height
                    color: Config.transparency ? "transparent" : "black"
                    Loader {
                        id: empty_background
                        anchors.top: parent.top
                        anchors.left: parent.left
                        width: parent.width
                        height: top_rect_height 
                        sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
                    }
                    Rectangle {
                        anchors.top: empty_background.bottom
                        width: parent.width
                        height: dataview.height - top_rect_height 
                        color: Config.transparency ? "transparent" : "black"
                    }
                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: Config.tr("Looks like there's no info for this location.")
                        font.pixelSize: huge_PixelSize 
                        color: Theme.secondaryColor
                        wrapMode: Text.Wrap
                        width: parent.width - 2*margin
                        //anchors.verticalCenter: parent.verticalCenter
                        anchors.top: parent.top
                        anchors.topMargin: 200*ratio
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: margin
                        anchors.rightMargin: margin
                    }
                    Button {
                        text: Config.tr("Try to update")
                        onClicked: {
                            main.update();
                          //  dataview.visible = (forecast_model_count(main_index) == 0 || current_model_count(main_index) == 0) ? true : false;
                          //  current_rect.visible = current_model_count(main_index) == 0 ? false : true;
                          //  list.visible = (forecast_model_count(main_index) == 0) ? false : true;
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 150*ratio
                    }

                }
                Rectangle {
                    id: current_rect
                    visible: current_model_count(main_index) == 0 ? false : true
                    anchors.top: station_rect.bottom
                    width: parent.width
                    height: top_rect_height 
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
                                height: current_text_rect_height 
                                anchors.top: parent.top
                                anchors.left: parent.left
                                color: Theme.primaryColor
                                text: main.current_model("current", main_index) == true ? Config.tr("Now") : Config.tr("Today")
                                font.pointSize: large_FontPointSize 
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter

                            }
                            Text {
                                anchors.top: parent.top
                                anchors.left: icon.right
                                id: temp_text
                                width: main.screen_width/2 - icon_size/2
                                height: current_text_rect_height 
                                color: Theme.primaryColor
                                text: model.temp + '°'
                                font.pointSize: large_FontPointSize
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                Component.onCompleted: {
                                    if (main.current_model("current", main_index) == true){
                                        if (main.current_model("temp", main_index) == "N/A"){
                                            temp_text.text = ""
                                            if (main.current_model("temp_high", main_index) != "N/A")
                                               temp_text.text =  main.current_model("temp_high", main_index) + '°'
                                            if ((main.current_model("temp_low", main_index)  != "N/A") && (main.current_model("temp_high", main_index) != "N/A"))
                                               temp_text.text =  temp_text.text + " / "
                                            if (main.current_model("temp_low", main_index)  != "N/A")
                                               temp_text.text = temp_text.text + main.current_model("temp_low", main_index) + '°'
                                            current_rect.color = "transparent"
                                        }else{
                                            temp_text.text = main.current_model("temp", main_index) + '°'
                                            current_rect.color = "transparent"
                                        }
                                    }else{
                                        if (main.forecast_model(index, "temp", main_index) == "N/A"){
                                            temp_text.text = ""
                                            if (main.forecast_model(index, "temp_high", main_index) != "N/A")
                                               temp_text.text =  main.forecast_model(index, "temp_high", main_index) + '°'
                                            if ((main.forecast_model(index, "temp_low", main_index)  != "N/A") && (main.forecast_model(index, "temp_high", main_index) != "N/A"))
                                               temp_text.text =  temp_text.text + " / "
                                            if (main.forecast_model(index, "temp_low", main_index)  != "N/A")
                                               temp_text.text = temp_text.text + main.forecast_model(index, "temp_low", main_index) + '°'
                                            current_rect.color = "transparent"
                                        }else{
                                            temp_text.text = main.forecast_model(index, "temp", main_index) + '°'
                                            current_rect.color = "transparent"
                                        }
                                    }
                                }
                            }
                            Rectangle {
                               id: desc  
                               height: description_text_rect_height 
                               color: Config.transparency ? "transparent" : "black"
                               width: current_rect.width 
                               anchors.left: parent.left
                               anchors.top: now.bottom
                               property color textColor: Theme.primaryColor
                               Row {  
                                   id: desc_row
                                   width: parent.width 
                                   Text { 
                                     id: text; 
                                     font.pointSize: description_FontPointSize; 
                                     width: parent.width 
                                     color: desc.textColor; 
                                     text: main.current_model("current", main_index) == true ? main.current_model("description", main_index) : main.forecast_model(index, "description", main_index) ; 
                                     verticalAlignment: Text.AlignVCenter; 
                                     horizontalAlignment: main.current_model("current", main_index) == true ? ((main.current_model("description", main_index) >35) ? Text.AlignHLeft : Text.AlignHCenter) : ((main.forecast_model(index, "description", main_index) >35) ? Text.AlignHLeft : Text.AlignHCenter); 
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
                                        from: 450*ratio; to: -500*ratio; 
                                        duration: 10000; 
                                        loops: Animation.Infinite; 
                                        running : main.current_model("current", main_index) == true ? ((main.current_model("description", main_index) >35) ? true : false) : ((main.forecast_model(index, "description", main_index) >35) ? true : false) ;  
                                    }  
                                }  
                            }
			    Image {
                                id: icon
                                source: main.current_model("current", main_index) == true ? Config.iconspath + "/" + Config.iconset + "/" + main.current_model("pict", main_index) : Config.iconspath + "/" + Config.iconset + "/" + main.forecast_model(index, "pict", main_index)
                                width: icon_size 
                                height: icon_size
                                anchors.top: parent.top
                                anchors.topMargin: -25
                                anchors.left: now.right
                                smooth: true    
                            }
                            Image {
                                id: humidity
                                source: Config.imagespath + "/humidity.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: small_box_rect_top_margin 
                                anchors.left: parent.left
                                anchors.leftMargin: margin
                                width: small_box_rect_height 
                                height: small_box_rect_height
                                smooth: true
                            }
                            Text {
                                text: main.current_model("current", main_index) == true ? (main.current_model("humidity", main_index) + "%") : (main.forecast_model(index, "humidity", main_index) + "%") 
                                anchors.left: humidity.right
                                anchors.leftMargin: small_box_rect_left_margin 
                                anchors.top: desc.bottom
                                anchors.topMargin: small_box_rect_top_margin 
                                height: small_box_rect_height 
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                id: wind_direction_background
                                source: Config.imagespath + "/wind_direction_background.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: small_box_rect_top_margin
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2
                                width: small_box_rect_height 
                                height: small_box_rect_height
                                smooth: true
                            }
                            Image {
                                id: wind_direction
                                source: Config.imagespath + "/wind_direction_arrow.png"
                                anchors.top: desc.bottom
                                anchors.topMargin: small_box_rect_top_margin
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2
                                width: small_box_rect_height 
                                height: small_box_rect_height 
                                smooth: true
                                transform: Rotation {
                                    origin.x: 15*ratio 
                                    origin.y: 15*ratio
                                    angle: main.current_model("current", main_index) == true ?  (main.current_model("wind_direction", main_index) == "N/A" ? 0 : main.getAngle(main.current_model("wind_direction", main_index))) : (main.forecast_model(index, "wind_direction", main_index) == "N/A" ? 0 : main.getAngle(main.forecast_model(index, "wind_direction", main_index)))
                                }
                            }
                            Text {
                                text: main.current_model("current", main_index) == true ? Config.tr(main.current_model("wind_direction", main_index)) : Config.tr(main.forecast_model(index, "wind_direction", main_index)) 
                                anchors.left: wind_direction.right
                                anchors.leftMargin: small_box_rect_left_margin 
                                anchors.top: desc.bottom
                                anchors.topMargin: small_box_rect_top_margin
                                height: small_box_rect_height 
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                    id: pressure
                                    source: Config.imagespath + "/pressure.png"
                                    anchors.top: humidity.bottom
                                    anchors.topMargin: small_box_rect_top_margin2 
                                    anchors.left: parent.left
                                    anchors.leftMargin: margin
                                    width: small_box_rect_height 
                                    height: small_box_rect_height 
                                    smooth: true
                            }
                            Text {
                                text: main.current_model("current", main_index) == true ?  main.current_model("pressure", main_index) : main.forecast_model(index, "pressure", main_index) + ' ' + Config.tr(Config.pressureunit)
                                anchors.left: pressure.right
                                anchors.leftMargin: small_box_rect_left_margin 
                                anchors.top: humidity.bottom
                                anchors.topMargin: small_box_rect_top_margin2 
                                height: small_box_rect_height 
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                id: wind_speed
                                source: Config.imagespath + "/wind_speed.png"
                                anchors.top: humidity.bottom
                                anchors.topMargin: small_box_rect_top_margin2 
                                anchors.left: parent.left
                                anchors.leftMargin: margin + main.width/2 
                                width: small_box_rect_height 
                                height: small_box_rect_height 
                                smooth: true
                            }
                            Text {
                                text: main.current_model("current", main_index) == true ? ((Config.windspeedunit == "Beaufort scale") ? main.current_model("wind_speed", main_index) : main.current_model("wind_speed", main_index) + ' ' + Config.tr(Config.windspeedunit)) : ((Config.windspeedunit == "Beaufort scale") ? main.forecast_model(index, "wind_speed", main_index) : main.forecast_model(index, "wind_speed", main_index) + ' ' + Config.tr(Config.windspeedunit))
                                anchors.left: wind_speed.right
                                anchors.leftMargin: small_box_rect_left_margin 
                                anchors.top: humidity.bottom
                                anchors.topMargin: small_box_rect_top_margin2 
                                height: small_box_rect_height 
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("current day onclicked");
                            Config._current_station_id(main.real_current_id)
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
                    visible: forecast_model_count(main_index) == 0 ? false : true
                    anchors.top: current_rect.bottom
                    model:  forecast_stub
                    delegate: itemDelegate
                    width: parent.width
                    height: row_rect_height * 21 
                    interactive: false
                    clip: true
                    Component.onCompleted: {
                       console.log("list onCompleted ", main_index, forecast_stub.count, Config.iconspath + "/" + Config.iconset)
                       console.log("WeatherStationDelegate.qml - Config.stationname ", Config.stationname) 
                       main.station_name_text = main.stationname1_index(main_index)
                       list.height = row_rect_height * forecast_model_count(main_index) 
                    }
                }
                Component {
                    id: itemDelegate
                    Item {
                        id: day
                        width: main_rectangle.width
                        height: row_rect_height 
                        ListView.onAdd: {
                            /* Hack */		
                            list.height = row_rect_height * forecast_model_count(main_index) 
                            forecast_stub.remove(list.count-1)
                        }		    
                        visible: main.forecast_model(index, "fulldate", main_index) == undefined ? false : true

                        Rectangle {
                            width: parent.width
                            height: row_rect_height
                            color: Config.transparency ? ((index % 2 != 0) ? "transparent" : "#10ffffff") : ((index % 2 != 0) ? "black" : "#0f0f0f")
                            Text {
                                id: txt_date
                                text: main.forecast_model(index, "fulldate", main_index) == undefined ? "" : main.forecast_model(index, "fulldate", main_index)
                                color: Theme.secondaryColor
                                font.pointSize: medium_FontPointSize 
                                anchors.left: parent.left
                                anchors.leftMargin: margin
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                text: main.forecast_model(index, "shortdate", main_index) == undefined ? "" : main.forecast_model(index, "shortdate", main_index) 
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                anchors.left: parent.left
                                anchors.leftMargin: (margin + txt_date.width + small_box_rect_left_margin)
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
                                source: main.forecast_model(index, "pict", main_index) == undefined ? Config.iconspath + "/" + Config.iconset + "/49.png" : Config.iconspath + "/" + Config.iconset + "/" +  main.forecast_model(index, "pict", main_index)
                                width: icon_size/2 
                                height: icon_size/2
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                smooth: true
                            }
                            Text {
                                id: txt_temphi
                                font.pointSize: medium_FontPointSize 
                                color: (Theme.primaryColor == "#ffffff") ? (main.forecast_model(index, "temp_high", main_index) != undefined ? getColor(main.forecast_model(index, "temp_high", main_index)) : getColor(0)) : Theme.primaryColor
                                text: main.forecast_model(index, "temp_high", main_index) + '°'
                                anchors.right: parent.right
                                anchors.rightMargin: margin + 70*ratio
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: txt_templo
                                font.pointSize: medium_FontPointSize 
                                color: Theme.secondaryColor
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
                                    Config._current_station_id(main.real_current_id)
                                    console.log(Config._current_station_id())
                                    main.updatemodels()

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
            height: 90*ratio
            color: Config.transparency ? Theme.secondaryHighlightColor : "black"
            opacity: 0.8
            Row {
                width: parent.width
                Label {
                    anchors.leftMargin: small_box_rect_left_margin
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: huge_PixelSize 
                    color: Theme.secondaryColor
                    font.family: faSolid.name
                    text: "\uf0c9"
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            console.log("click to menu")
                            drawer.open()
                        }
                    }
                }
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
        Timer {
            id: moveTimer
            running: !flipmoving && Qt.application.active
            interval: 500
            repeat: true
            /*
            onRunningChanged: {
                left_indicator.brightness = 1.0
                left_indicator.falloffRadius = 0.05
            }
            onTriggered: {
                left_indicator.falloffRadius = left_indicator.falloffRadius < 0.01 ? 0.2 : 0.075
                left_indicator.brightness = left_indicator.brightness < 0.5 ? 1.0 : 0.4
            }
            */
        }
        Timer {
            id: stopTimer
            running: !flipmoving && Qt.application.active 
            interval: 3000
            repeat: false
            onTriggered: {
                moveTimer.running = false
            }
        }
    }
}

