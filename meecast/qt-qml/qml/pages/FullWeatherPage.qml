import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0

Page {
    id: fullweather
    property int screen_height : fullweather.height
    property int screen_width : fullweather.width 
//    property int screen_height : (isPortrait) ? fullweather.height : fullweather.width
//    property int screen_width : (isPortrait) ? fullweather.width :  fullweather.height

    //property real ratio: fullweather.screen_width/540
    property real ratio: Theme.pixelRatio
    property int icon_size: 128*ratio
    property int top_rect_height: 274*ratio
    property int current_text_rect_height: 84*ratio
    property int large_FontPointSize: 26*ratio
    property int description_text_rect_height: 44*ratio
    property int description_FontPointSize: 20*ratio
    property int medium_FontPointSize: 18*ratio
    property int small_FontPointSize: 14*ratio
    property int row_rect_height: 80*ratio
    property int toolbar_height: 120*ratio
    property int margin: 16*ratio
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

    function check_hours(){
        var i = 0;
        var result = false;
        console.log("check_hours() day", day);
        model_hours.update_model(4);
        var fulldate = model_day.getdata(day, "fulldate");
        while (i<model_hours.rowCount()){   
            if (model_hours.getdata(i, "fulldate") == fulldate){
                result = true;
            }else{
                model_hours.remove(i)
                i--;
            }
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
            if (Config.transparency)
                current_rect.color = "transparent"
            else
                /* current_rect.color = getColor(model_current.getdata(day, "temp_high"));
                not sure what to do here, with the coloring it looks aweful on black theme */
                current_rect.color = "transparent"
            description_text = model_current.getdata(day, "description") ? model_current.getdata(day, "description") : ""
	   
            if ((model_current.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_current.getdata(day, "humidity")+'%'});
            if ((model_current.getdata(day, "wind_direction")) != "" && (model_current.getdata(day, "wind_direction")) != "N/A")
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
            if ((model_current.getdata(day, "precipitation")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation:"),
                         value: model_current.getdata(day, "precipitation") + Config.tr("mm")});
            if ((model_current.getdata(day, "precipitation_rate")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation rate:"),
                         value: model_current.getdata(day, "precipitation_rate") + ' ' + Config.tr("mm/h")});
            if ((model_current.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_current.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_current.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_current.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_current.getdata(day, "dewpoint")) != "N/A")
                condition.append({cond_name: Config.tr("Dew point") + ":",
                         value: model_current.getdata(day, "dewpoint") + '°' + Config.temperatureunit});
/*                         
            if ((model_current.getdata(day, "map_pattern")) != ""){
                map_pattern = model_current.getdata(day, "map_pattern")
                map_text.visible = true;
                mapbackground_pattern = model_current.getdata(day, "mapbackground_pattern")
                count_of_maps = model_current.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;
*/
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
            if (Config.transparency)
                current_rect.color = "transparent"
            else
                /* current_rect.color = getColor(model_day.getdata(day, "temp_high"));
                not sure what to do here, with the coloring it looks aweful on black theme */
                current_rect.color = "transparent"
            description_text = model_day.getdata(day, "description") ? model_day.getdata(day, "description") : ""
	   
            if ((model_day.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_day.getdata(day, "humidity")+'%'});
            if ((model_day.getdata(day, "wind_direction")) != "" && (model_day.getdata(day, "wind_direction")) != "N/A")
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
            if ((model_day.getdata(day, "precipitation")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation:"),
                         value: model_day.getdata(day, "precipitation") + Config.tr("mm")});
            if ((model_day.getdata(day, "precipitation_rate")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation rate:"),
                         value: model_day.getdata(day, "precipitation_rate") + ' ' + Config.tr("mm/h")});
            if ((model_day.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_day.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_day.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_day.getdata(day, "dewpoint")) != "N/A")
                condition.append({cond_name: Config.tr("Dew point") + ":",
                         value: model_day.getdata(day, "dewpoint") + '°' + Config.temperatureunit});
            if ((model_day.getdata(day, "temp")) != "N/A")
                temperature.text =  model_day.getdata(day, "temp") + '°'
	        else{
                if ((model_day.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_day.getdata(day, "temp_high") + '°'
            }
/*            
            if ((model_day.getdata(day, "map_pattern")) != ""){
                map_pattern = model_day.getdata(day, "map_pattern")
                mapbackground_pattern = model_day.getdata(day, "mapbackground_pattern")
                map_text.visible = true;
                count_of_maps = model_day.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;
*/                
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
            if (Config.transparency)
                current_rect.color = "transparent"
            else
                /* current_rect.color = getColor(model_day.getdata(day, "temp_low"));
                not sure what to do here, with the coloring it looks aweful on black theme */
                current_rect.color = "transparent"
            description_text = model_night.getdata(day, "description") ? model_night.getdata(day, "description") : ""
            if ((model_night.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity") + ":",
                         value: model_night.getdata(day, "humidity")+'%'});
            if ((model_night.getdata(day, "wind_direction")) != "" && (model_night.getdata(day, "wind_direction")) != "N/A")
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
            if ((model_night.getdata(day, "precipitation")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation:"),
                         value: model_night.getdata(day, "precipitation") + Config.tr("mm")});
            if ((model_night.getdata(day, "precipitation_rate")) != "N/A")
                condition.append({cond_name: Config.tr("Precipitation rate:"),
                         value: model_night.getdata(day, "precipitation_rate") + ' ' + Config.tr("mm/h")});
            if ((model_night.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_night.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike") + ":",
                         value: model_night.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_night.getdata(day, "dewpoint")) != "N/A")
                condition.append({cond_name: Config.tr("Dew point") + ":",
                         value: model_night.getdata(day, "dewpoint") + '°' + Config.temperatureunit});
            if ((model_night.getdata(day, "temp")) != "N/A")
                temperature.text =  model_night.getdata(day, "temp") + '°'
	        else{
                if ((model_night.getdata(day, "temp_low")) != "N/A")
                    temperature.text =  model_night.getdata(day, "temp_low") + '°'
            }
/*            
            if ((model_night.getdata(day, "map_pattern")) != ""){
                map_pattern = model_night.getdata(day, "map_pattern")
                mapbackground_pattern = model_night.getdata(day, "mapbackground_pattern")
                map_text.visible = true;
                count_of_maps = model_night.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;
*/
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
            hours_condition.clear();
            flickable.contentHeight = row_rect_height * model_hours.rowCount() + day_rect.height;
            var i = 0;
            while (i<model_hours.rowCount()){   
                var hour_data = ""
                if (timeFormatConfig.value === "24") {
                    hour_data=model_hours.getdata(i, 'hourdate24')
                }else{
                    hour_data=model_hours.getdata(i, 'hourdate12')
                }
                hours_condition.append({fulldate: model_hours.getdata(i, 'fulldate'), hourdate: hour_data,
                                        pict: model_hours.getdata(i, 'pict'), precipitation: model_hours.getdata(i, 'precipitation'),
                                        temp: model_hours.getdata(i, 'temp'), temp_high: model_hours.getdata(i, 'temp_high')});
                i++;
            }
            hours_list.height = row_rect_height * hours_condition.rowCount();

	}
        if ((model_day.getdata(day, "sunrise")) != "N/A")

            if (timeFormatConfig.value === "24") 
                condition2.append({cond_name: Config.tr("Sunrise:"),
                         value: model_day.getdata(day, "sunrise24")});
            else
                condition2.append({cond_name: Config.tr("Sunrise:"),
                         value: model_day.getdata(day, "sunrise12")});
        if ((model_day.getdata(day, "sunset")) != "N/A")
            if (timeFormatConfig.value === "24") 
                condition2.append({cond_name: Config.tr("Sunset:"),
                         value: model_day.getdata(day, "sunset24")});
            else
                condition2.append({cond_name: Config.tr("Sunset:"),
                         value: model_day.getdata(day, "sunset12")});
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

    ConfigurationValue {
        id: timeFormatConfig
        key: "/sailfish/i18n/lc_timeformat24h"
    }

    Rectangle{
        width: screen_width
        height: screen_height 
        color: {
            if (Config.transparency == "transparent"){
                return "transparent"
            }else{
                if (Theme.primaryColor == "#000000"){
                    return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                }else{
                    return "black"
                }
            }
        }

        SilicaFlickable {
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
                height: screen_height*0.1 
                color: "transparent"

            Rectangle {
                id: current_rect
                anchors.top: day_rect.bottom
                width: parent.width
                height: screen_height + toolbar_height
                //color: "black"
                color: "transparent"
                Rectangle {
                    height: top_rect_height
                    width: parent.width
                    color: "transparent"
                    Loader {
                        anchors.fill: parent
                        sourceComponent: Image {source: Config.imagespath + "/mask_background_main.png"}
                    }
                }

                Text {
                    id: now
                    width: screen_width/2 - icon_size/2 
                    height: current_text_rect_height 
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    color: Theme.primaryColor
                    text: day_period_name;
                    font.pointSize: large_FontPointSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Image {
                    id: icon
                    source:  image_source 
                    width: icon_size 
                    height: icon_size 
                    anchors.top: parent.top
                    anchors.topMargin: -22*ratio
                    anchors.left: now.right
                     smooth: true
                }
                Text {
                    id: temperature
                    anchors.top: parent.top
                    anchors.left: icon.right
                    anchors.rightMargin: margin
                    width: screen_width/2 - icon_size/2 
                    height: current_text_rect_height 
                    color: Theme.primaryColor
                    font.pointSize: large_FontPointSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Rectangle {
                   id: desc  
                   height: description_text_rect_height
                   color: "transparent"
                   width: parent.width 
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
                            id: text_anim; from: 450*ratio; to: -500*ratio ; 
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
                    anchors.topMargin: 20*ratio
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: parent.width - 2*margin
                    height: 260*ratio
                    cellWidth: (parent.width - 2*margin) / 2
                    cellHeight: (condition.count > 6) ? 64*ratio : 94*ratio
                    model: condition
                    interactive: false
                    clip: true
                    delegate: Column {
                        width: grid.width / 2
                        spacing: 3
                        Text {
                            text: model.cond_name
                            color: Theme.secondaryColor
//                            color: "#999999"
                            font.pointSize: medium_FontPointSize 
                        }
                        Text {
                            text: model.value
                            color: Theme.primaryColor
//                            color: "white"
                            font.pointSize: medium_FontPointSize
                        }
                    }
                }
                Rectangle {
                    id: splitter
                    color: Config.transparency ? "#cfcfcf" : "#303030"
                    x: 20*ratio; width: parent.width - 40*ratio; height: 2*ratio
                    anchors.top: grid.bottom 
                    anchors.leftMargin: 20*ratio
                }
                ListModel {
                    id: condition2
                }
                GridView {
                    id: grid2
                    anchors.top: splitter.bottom
                    anchors.topMargin: 10*ratio
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: parent.width - 2*margin
                    height: 165*ratio
                    cellWidth: (parent.width - 2*margin) / 2
                    model: condition2
                    interactive: false
                    clip: true
                    delegate: Column {
                        width: grid.width / 2
                        spacing: 3
                        Text {
                            text: model.cond_name
                            color: Theme.secondaryColor
                            font.pointSize: medium_FontPointSize 
                        }
                        Text {
                            text: model.value
                            color: Theme.primaryColor
                            font.pointSize: medium_FontPointSize
                        }
                    }
                }
                Rectangle {
                   id: map_rect  
                   visible: (count_of_maps > 0) ? true : false
                   height: description_text_rect_height
                   color: "transparent"
                   width: parent.width 
                   anchors.left: parent.left
                   anchors.top: grid2.bottom 
                   anchors.topMargin: 20*ratio
                   Text {
                        id: map_text
                        anchors.fill: parent
                        text:  Config.tr("Show on Map") 
                        color: Theme.primaryColor
                        visible: false
                        font.pointSize: large_FontPointSize 
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
            ListModel {
                id: hours_condition
            }

            ListView {
                    id: hours_list
                    visible: false 
                    anchors.top: day_rect.bottom
                    model: hours_condition 
                    delegate: itemDelegate
                    width: parent.width
                    height: row_rect_height * 24 
                    interactive: false
                    clip: true
            }
            Component {
                    id: itemDelegate
                    Item {
                        id: day
                        width: parent.width
                        height: row_rect_height 
                        Rectangle {
                            width: parent.width
                            height: row_rect_height 
                            color: {
                                 if (Config.transparency){
                                    if (index % 2 != 0) {
                                        return "transparent"
                                    }else{
                                        return "#10ffffff"
                                    }
                                 }else{
                                    if (Theme.primaryColor == "#000000"){
                                        if (index % 2 != 0) {
                                           return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                                        }else{
                                           return  Theme.rgba(Theme.overlayBackgroundColor, Theme.highlightBackgroundOpacity)
                                        }
                                    }else{
                                        if (index % 2 != 0) {
                                           return "black"
                                        }else{
                                           return "#0f0f0f"
                                        }
                                    }
                                 }
                            }

                            Text {
                                id: txt_date
                                text: model.fulldate
                                color: Theme.secondaryColor
                                font.pointSize: medium_FontPointSize 
                                anchors.left: parent.left
                                anchors.leftMargin: margin
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                text: model.hourdate
                                color: Theme.primaryColor
                                font.pointSize: medium_FontPointSize 
                                anchors.left: parent.left
                                anchors.leftMargin: (margin + txt_date.width + 8*ratio)
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Image {
				id: horly_icon
                                source: Config.iconspath + "/" + Config.iconset + "/" + model.pict
                                width: icon_size/2 
                                height: icon_size/2
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                smooth: true
                            }
                            Text {
                                id: txt_precipitation
                                visible: (model.precipitation == "N/A") ? false : true
                                font.pointSize: small_FontPointSize 
                                color: (model.precipitation == "0.0") ? "#889397" : Theme.primaryColor
                                text: model.precipitation + Config.tr("mm")
                                anchors.left: parent.left 
                                anchors.leftMargin: screen_width/2 + icon_size/4 + 30 
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            Text {
                                id: txt_temp
                                font.pointSize: medium_FontPointSize 
                                color: (Theme.primaryColor == "#ffffff") ? getColor(model.temp_high) : Theme.primaryColor
                                text: model.temp + '°'
                                anchors.right: parent.right
                                anchors.rightMargin: (model.precipitation == "N/A") ? margin + 70*ratio : margin + 30*ratio
                                height:parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                } //component itemDelegate
            }
        }
        Rectangle{
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: flickable.botttom
            color: {
                 if (Config.transparency){
                    return Theme.secondaryHighlightColor
                 }else{
                    if (Theme.primaryColor == "#000000"){
                        return  Theme.rgba(Theme.overlayBackgroundColor, Theme.highlightBackgroundOpacity)
                    }else{
                        return "black"
                    }
                 }
            }
            opacity: 0.8
            id: toolbar
            height: toolbar_height 
            Row {
                spacing: Theme.paddingLarge
                //spacing: Theme.itemSizeSmall
                anchors.horizontalCenter: parent.horizontalCenter
                Switch {
                    y: -0.028*screen_height 
                    id: "toolbarnow"
                    icon.source: Config.imagespath + "/now.png" 
                    visible: (current && day == 0) ? true : false
                    onClicked: {
                         day_period = "current";
                         updateperiod();
                    }
                }
                Switch {
                    y: -0.028*screen_height 
                    id: "toolbarday"
                    icon.source:  Config.imagespath + "/day.png"
                    onClicked: {
                         day_period = "day";
                         updateperiod();
                    }
                }
                Switch {
                    y: -0.028*screen_height 
                    id: "toolbarnight"
                    icon.source:  Config.imagespath + "/night.png"
                    onClicked: {
                         day_period = "night";
                         updateperiod();
                    }
                }
                Switch {
                    y: -0.028*screen_height 
                    id: "toolbarclock"
                    visible: check_hours()
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
