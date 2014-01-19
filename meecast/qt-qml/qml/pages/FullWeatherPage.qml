import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: fullweather
    property int screen_height : 960
    property int screen_width : 540
    property int margin: 16
    property int day: 0
    property bool current: false
    property string day_period: "day"
    property string day_period_name: ""
    property string image_source: ""
    property string description_text: ""
    property variant description_text_alignment: Text.AlignHLeft;
    property string map_pattern: "";  
    property string mapbackground_pattern: "";  
    property string count_of_maps: "0";  

    property variant model_current: Current 
    property variant model_day:  Forecast_model
    property variant model_night:  (current) ? Current_night : Forecast_night_model
    property variant model_hours:  Forecast_hours_model


    function check_hours (){
        var i = 0;
        var result = 0;
        var fulldate = model_day.getdata(day, "fulldate");
        while (i<model_hours.rowCount()){   
            if (model_hours.getdata(i, "fulldate") == fulldate)
		        result = 1;
	        i++;
        }
        return result;
    }

    function updateperiod(){
        condition.clear()
        condition2.clear()
	    if (day_period == "current"){
	        toolbarnow.checked = true
	        toolbarday.checked = false 
	        toolbarnight.checked = false
	        toolbarclock.checked = false
            day_rect.visible = true;
            current_rect.visible = true;
            hours_list.visible = false;
            flickable.contentHeight = day_rect.height + current_rect.height;

            day_period_name = Config.tr("Now")
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_current.getdata(day, "pict")
            current_rect.color = getColor(model_current.getdata(day, "temp_high"));
            description_text = model_current.getdata(day, "description") ? model_current.getdata(day, "description") : ""
	   
            if ((model_current.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_current.getdata(day, "humidity")+'%'});
            if ((model_current.getdata(day, "wind_direction")) != "")
                condition.append({cond_name: Config.tr("Wind direction") + ":",
                         value: Config.tr(model_current.getdata(day, "wind_direction"))});
            if ((model_current.getdata(day, "pressure")) != "N/A")
                condition.append({cond_name: Config.tr("Pressure:"),
                         value: model_current.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_current.getdata(day, "wind_speed")) != "N/A"){
                if (Config.windspeedunit == "Beaufort scale"){
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_current.getdata(day, "wind_speed") + ' (' + Config.tr(Config.windspeedunit) + ')'});
                }else{
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_current.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
                }
            }
            if ((model_current.getdata(day, "ppcp")) != "N/A")
                condition.append({cond_name: Config.tr("Ppcp:"),
                         value: model_current.getdata(day, "ppcp") + '%'});
            if ((model_current.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_current.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_current.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_current.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_current.getdata(day, "map_pattern")) != ""){
                map_pattern = model_current.getdata(day, "map_pattern")
                map_text.visible = true;
                mapbackground_pattern = model_current.getdata(day, "mapbackground_pattern")
                count_of_maps = model_current.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;

            if ((model_current.getdata(day, "temp")) != "N/A")
                temperature.text =  model_current.getdata(day, "temp") + '°'
     	    else{
                if ((model_current.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_current.getdata(day, "temp_high") + '°'
            }
            if ((model_current.getdata(day, "visible")) != "N/A")
                condition.append({cond_name: Config.tr("Visible:"),
                         value: model_current.getdata(day, "visible") + ' ' + Config.tr(Config.visibleunit)});
            if ((model_current.getdata(day, "uv_index")) != "N/A")
                condition.append({cond_name: Config.tr("UV index:"),
                         value: model_current.getdata(day, "uv_index")});
	}

        if (day_period == "day"){
	        toolbarnow.checked = false 
	        toolbarday.checked = true
	        toolbarnight.checked = false
	        toolbarclock.checked = false
            day_rect.visible = true;
            current_rect.visible = true;
            hours_list.visible = false;
            flickable.contentHeight = day_rect.height + current_rect.height;

            day_period_name = Config.tr("Day")
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_day.getdata(day, "pict")
            current_rect.color = getColor(model_day.getdata(day, "temp_high"));
            description_text = model_day.getdata(day, "description") ? model_day.getdata(day, "description") : ""
	   
            if ((model_day.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_day.getdata(day, "humidity")+'%'});
            if ((model_day.getdata(day, "wind_direction")) != "")
                condition.append({cond_name: Config.tr("Wind direction") + ":",
                         value: Config.tr(model_day.getdata(day, "wind_direction"))});
            if ((model_day.getdata(day, "pressure")) != "N/A")
                condition.append({cond_name: Config.tr("Pressure:"),
                         value: model_day.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_day.getdata(day, "wind_speed")) != "N/A")
                if (Config.windspeedunit == "Beaufort scale"){
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_day.getdata(day, "wind_speed") + ' (' + Config.tr(Config.windspeedunit) + ')'});
                }else{
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_day.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
                }
            if ((model_day.getdata(day, "ppcp")) != "N/A")
                condition.append({cond_name: Config.tr("Ppcp:"),
                         value: model_day.getdata(day, "ppcp") + '%'});
            if ((model_day.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_day.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_day.getdata(day, "flike") + '°' + Config.temperatureunit});

            if ((model_day.getdata(day, "temp")) != "N/A")
                temperature.text =  model_day.getdata(day, "temp") + '°'
	        else{
                if ((model_day.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_day.getdata(day, "temp_high") + '°'
            }
            if ((model_day.getdata(day, "map_pattern")) != ""){
                map_pattern = model_day.getdata(day, "map_pattern")
                mapbackground_pattern = model_day.getdata(day, "mapbackground_pattern")
                map_text.visible = true;
                count_of_maps = model_day.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;
	}
	if (day_period == "night"){
            day_period_name = Config.tr("Night");
            toolbarnow.checked = false;
            toolbarnight.checked = true;
            toolbarday.checked = false;
            toolbarclock.checked = false;
            day_rect.visible = true;
            current_rect.visible = true;
            hours_list.visible = false;
            flickable.contentHeight = day_rect.height + current_rect.height;

            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_night.getdata(day, "pict");
            current_rect.color = getColor(model_day.getdata(day, "temp_low"));
            description_text = model_night.getdata(day, "description") ? model_night.getdata(day, "description") : ""
            if ((model_night.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_night.getdata(day, "humidity")+'%'});
            if ((model_night.getdata(day, "wind_direction")) != "")
                condition.append({cond_name: Config.tr("Wind direction") + ":",
                         value: Config.tr(model_night.getdata(day, "wind_direction"))});
            if ((model_night.getdata(day, "pressure")) != "N/A")
                condition.append({cond_name: Config.tr("Pressure:"),
                         value: model_night.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_night.getdata(day, "wind_speed")) != "N/A")
                if (Config.windspeedunit == "Beaufort scale"){
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_night.getdata(day, "wind_speed") + ' (' + Config.tr(Config.windspeedunit) + ')'});
                }else{
                    condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_night.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
                }
            if ((model_night.getdata(day, "ppcp")) != "N/A")
                condition.append({cond_name: Config.tr("Ppcp:"),
                         value: model_night.getdata(day, "ppcp") + '%'});
            if ((model_night.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_night.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_night.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_night.getdata(day, "temp")) != "N/A")
                temperature.text =  model_night.getdata(day, "temp") + '°'
	        else{
                if ((model_night.getdata(day, "temp_low")) != "N/A")
                    temperature.text =  model_night.getdata(day, "temp_low") + '°'
            }
            if ((model_night.getdata(day, "map_pattern")) != ""){
                map_pattern = model_night.getdata(day, "map_pattern")
                mapbackground_pattern = model_night.getdata(day, "mapbackground_pattern")
                map_text.visible = true;
                count_of_maps = model_night.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;

	}
	if (day_period == "hours"){
            day_period_name = Config.tr("Hours");
	        toolbarnow.checked = false;
            toolbarnight.checked = false;
            toolbarday.checked = false;
            toolbarclock.checked = true;
            day_rect.visible = true;
            current_rect.visible = false;
            hours_list.visible = true;
            flickable.contentHeight = hours_list.height + day_rect.height;

	}
        if ((model_day.getdata(day, "sunrise")) != "N/A")
            condition2.append({cond_name: Config.tr("Sunrise:"),
                         value: model_day.getdata(day, "sunrise")});
        if ((model_day.getdata(day, "sunset")) != "N/A")
            condition2.append({cond_name: Config.tr("Sunset:"),
                         value: model_day.getdata(day, "sunset")});
        if ((model_day.getdata(day, "daylength")) != "N/A")
            condition2.append({cond_name: Config.tr("Day length:"),
                         value: model_day.getdata(day, "daylength")});
        if ((model_day.getdata(day, "lastupdate")) != "N/A")
            condition2.append({cond_name: Config.tr("Last update:"),
                         value: model_day.getdata(day, "lastupdate")});
	
	if (description_text.length > 35){
	    description_text_alignment = Text.AlignHLeft
	}else{
	    description_text_alignment = Text.AlignHCenter
	}

    }

 
    function getColor(t){
        var c1, c2, c3;
        if (Config.temperatureunit == "F"){
            t = (t - 32) * 5 / 9;
        }
        if (t >= 30){
            c2 = (t - 50)*(246/255-60/255)/(30-50) + 60/255;
            return Qt.rgba(1, c2, 0, 1);
        }else if (t < 30 && t >= 15){
            c1 = (t - 30)*(114/255-1)/(15-30) + 1;
            c2 = (t - 30)*(1-246/255)/(15-30) + 246/255;
            return Qt.rgba(c1, c2, 0, 1);
        }else if (t < 15 && t >= 0){
            c1 = (t - 15)*(1-114/255)/(0-15) + 144/255;
            c3 = (t - 15)*(1-0)/(0-15) + 0;
            return Qt.rgba(c1, 1, c3, 1);
        }else if (t < 0 && t >= -15){
            c1 = (t - 0)*(0-1)/(-15-0) + 1;
            c2 = (t - 0)*(216/255-1)/(-15-0) + 1;
            return Qt.rgba(c1, c2, 1, 1);
        }else if (t < -15 && t >= -30){
            c2 = (t - (-15))*(66/255-216/255)/(-30+15) + 216/255;
            return Qt.rgba(0, c2, 1, 1);
        }else if (t < -30){
            c1 = (t - (-30))*(132/255-0)/(-30+15) + 0;
            c2 = (t - (-30))*(0-66/255)/(-30+15) + 66/255;
            return Qt.rgba(c1, c2, 1, 1);
        }
    }

    Rectangle{
        width: screen_width
        height: screen_height 
        color: "black"
        Flickable {
            id: flickable
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: toolbar.top
            flickableDirection: Flickable.VerticalFlick
            contentWidth: flickable.width
            PageHeader {
                id: daynameheader
                title: (current && day == 0) ? Config.tr("Today") : model_day.getdata(day, "date");
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (day < model_day.rowCount()-1){ 
                            console.log("next day");
                            day_period="day";
                            day++;
                            fullweather.updateperiod();
                        }
                    }
                }
            }

            Rectangle {
                id: day_rect
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: 92
                color: "transparent"

            Rectangle {
                id: current_rect
                anchors.top: day_rect.bottom
                width: parent.width
                height: 274
                //color: "black"
                color: "transparent"
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_background_main.png"}
                }

                Text {
                    id: now
                    width: 160
                    height: 84
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    color: "white"
                    text: day_period_name;
                    font.pointSize: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Image {
                    id: icon
                    source:  image_source 
                    width: 128
                    height: 128
                    anchors.top: parent.top
                    anchors.topMargin: -22
                    anchors.left: now.right
                     smooth: true
                }
                Text {
                    id: temperature
                    anchors.top: parent.top
                    anchors.left: icon.right
                    anchors.rightMargin: margin
                    width: 160
                    height: 84
                    color: "white"
                    font.pointSize: 26
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Rectangle {
                   id: desc  
                   height: 44
                   color: "transparent"
                   width: parent.width 
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
                            text: description_text ; 
                            verticalAlignment: Text.AlignVCenter; 
                            horizontalAlignment: description_text_alignment; 
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
                            id: text_anim; from: 450; to: -500 ; 
                            duration: 10000; loops: Animation.Infinite; 
                            running : false; 
                        } 
                   }  
                }

                ListModel {
                    id: condition
                }
                Component.onCompleted: {
                    updateperiod();
                }
                GridView {
                    id: grid
                    anchors.top: desc.bottom
                    anchors.topMargin: 20
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: parent.width - 2*margin
                    height: 260
                    cellWidth: (parent.width - 2*margin) / 2
                    cellHeight: (condition.count > 6) ? 64 : 94
                    model: condition
                    interactive: false
                    clip: true
                    delegate: Column {
                        width: grid.width / 2
                        spacing: 3
                        Text {
                            text: model.cond_name
                            color: "#999999"
                            font.pointSize: 18
                        }
                        Text {
                            text: model.value
                            color: "white"
                            font.pointSize: 18
                        }
                    }
                }
                Rectangle {
                    id: splitter
                    color: "#303030"
                    x: 20; width: parent.width - 40; height: 2
                    anchors.top: grid.bottom 
                    anchors.leftMargin: 20
                }
                ListModel {
                    id: condition2
                }
                GridView {
                    id: grid2
                    anchors.top: splitter.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: parent.width - 2*margin
                    height: 165
                    cellWidth: (parent.width - 2*margin) / 2
                    model: condition2
                    interactive: false
                    clip: true
                    delegate: Column {
                        width: grid.width / 2
                        spacing: 3
                        Text {
                            text: model.cond_name
                            color: "#999999"
                            font.pointSize: 18
                        }
                        Text {
                            text: model.value
                            color: "white"
                            font.pointSize: 18
                        }
                    }
                }
                Rectangle {
                   id: map_rect  
                   visible: (count_of_maps > 0) ? true : false
                   height: 44
                   color: "transparent"
                   width: parent.width 
                   anchors.left: parent.left
                   anchors.top: grid2.bottom 
                   anchors.topMargin: 20
                   Text {
                        id: map_text
                        anchors.fill: parent
                        text:  Config.tr("Show on Map") 
                        color: "white"
                        visible: false
                        font.pointSize: 24 
                        width: parent.width 
                        horizontalAlignment: Text.AlignHCenter
                   }
                   MouseArea {
                        anchors.fill: parent
                        onClicked: {
                               console.log("Map onclicked");
                               pageStack.push(Qt.resolvedUrl("MapPage.qml"),
                                              {map_pattern: map_pattern, count_of_maps: count_of_maps,
                                               mapbackground_pattern: mapbackground_pattern }
                                             )
                        }
                   }
                }
            }
            ListView {
                    id: hours_list
                    visible: false 
                    anchors.top: day_rect.bottom
                    model: Forecast_hours_model 
                    delegate: itemDelegate
                    width: parent.width
                    height: 80 * Forecast_hours_model.rowCount()
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
                                text: model.hourdate
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
                                id: txt_temp
                                font.pointSize: 18
                                color: getColor(temp_high)
                                text: model.temp + '°'
                                anchors.right: parent.right
                                anchors.rightMargin: margin + 70
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                } //component itemDelegate
            }
        }
        Rectangle{
            color: "black"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: flickable.botttom
            id: toolbar
            height: 110 
            Row {
                spacing: Theme.paddingLarge
                //spacing: Theme.itemSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                Switch {
                    id: "toolbarnow"
                    icon.source: Config.imagespath + "/now.png" 
                    visible: (current && day == 0) ? true : false
                    onClicked: {
                         day_period = "current";
                         updateperiod();
                    }
                }
                Switch {
                    id: "toolbarday"
                    icon.source:  Config.imagespath + "/day.png"
                    onClicked: {
                         day_period = "day";
                         updateperiod();
                    }
                }
                Switch {
                    id: "toolbarnight"
                    icon.source:  Config.imagespath + "/night.png"
                    onClicked: {
                         day_period = "night";
                         updateperiod();
                    }
                }
                Switch {
                    id: "toolbarclock"
                    icon.source:  Config.imagespath + "/clock.png"
                    onClicked: {
                         day_period = "hours";
                         updateperiod();
                    }
                }
            }
        }
    }
}

