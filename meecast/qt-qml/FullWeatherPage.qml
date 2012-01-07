//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: fullweather
    property int margin: 16
    property int day: 0
    property bool current: false
    property string day_period: "day"
    property string day_period_name: ""
    property string image_source: ""
    property string description_text: ""

    property variant model_day:  (current) ? Current : Forecast_model
    property variant model_night:  (current) ? Current_night : Forecast_night_model
    property variant model_hours:  Forecast_hours_model

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }
        ButtonRow{
        ToolButton {
            id: "toolbarday"
            platformStyle: TabButtonStyle{}
            onClicked: {
                day_period = "day";
                updateperiod();
            }
            iconSource:  Config.imagespath + "/day.png"
	    flat: true 
	    checkable: true
	    checked: true 
        }
        ToolButton {
            id: "toolbarnight"
            platformStyle: TabButtonStyle{}
            onClicked: {
                day_period = "night";
                updateperiod();
            }
            iconSource:  Config.imagespath + "/night.png"
	    flat: true
	    checkable: true
	    checked: flase 
        }
        ToolButton {
            id: "toolbarclock"
            platformStyle: TabButtonStyle{}
            onClicked: {
                day_period = "clock";
                updateperiod();
            }
            iconSource:  Config.imagespath + "/clock.png"
	    flat: true
	    checkable: true
	    checked: flase 
        }

}
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: {(myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()}
            anchors.right: parent == undefined ? undefined : parent.right
        }

   }
    orientationLock: PageOrientation.LockPortrait
    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }
    function updateperiod()
    {
        condition.clear()
	condition2.clear()
        if (day_period == "day"){
	    toolbarday.checked = true
	    toolbarnight.checked = false
	    day_period_name = Config.tr("Day")
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_day.getdata(day, "pict")
            current_rect.color = getColor(model_day.getdata(day, "temp_high"));
            description_text = model_day.getdata(day, "description") ? model_day.getdata(day, "description") : ""

	   
            if ((model_day.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity:"),
                         value: model_day.getdata(day, "humidity")+'%'});
            if ((model_day.getdata(day, "wind_direction")) != "")
                condition.append({cond_name: Config.tr("Wind direction:"),
                         value: Config.tr(model_day.getdata(day, "wind_direction"))});
            if ((model_day.getdata(day, "pressure")) != "N/A")
                condition.append({cond_name: Config.tr("Pressure:"),
                         value: model_day.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_day.getdata(day, "wind_speed")) != "N/A")
                condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_day.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "ppcp")) != "N/A")
                condition.append({cond_name: Config.tr("Ppcp:"),
                         value: model_day.getdata(day, "ppcp")} + '%');
            if ((model_day.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_day.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike:"),
                         value: model_day.getdata(day, "flike") + '°' + Config.temperatureunit});

            if ((model_day.getdata(day, "temp")) != "N/A")
                temperature.text =  model_day.getdata(day, "temp") + '°'
	    else{
                if ((model_day.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_day.getdata(day, "temp_high") + '°'
            }
	}
	if (day_period == "night"){
            day_period_name = Config.tr("Night");
            toolbarnight.checked = true;
            toolbarday.checked = false;
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_night.getdata(day, "pict");
            current_rect.color = getColor(model_day.getdata(day, "temp_low"));
            description_text = model_night.getdata(day, "description") ? model_night.getdata(day, "description") : ""
            if ((model_night.getdata(day, "humidity")) != "N/A")
                condition.append({cond_name: Config.tr("Humidity:"),
                         value: model_night.getdata(day, "humidity")+'%'});
            if ((model_night.getdata(day, "wind_direction")) != "")
                condition.append({cond_name: Config.tr("Wind direction:"),
                         value: Config.tr(model_night.getdata(day, "wind_direction"))});
            if ((model_night.getdata(day, "pressure")) != "N/A")
                condition.append({cond_name: Config.tr("Pressure:"),
                         value: model_night.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_night.getdata(day, "wind_speed")) != "N/A")
                condition.append({cond_name: Config.tr("Wind speed") + ":",
                         value: model_night.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "ppcp")) != "N/A")
                condition.append({cond_name: Config.tr("Ppcp:"),
                         value: model_night.getdata(day, "ppcp")} + '%');
            if ((model_night.getdata(day, "wind_gust")) != "N/A")
                condition.append({cond_name: Config.tr("Wind gust:"),
                         value: model_night.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "flike")) != "N/A")
                condition.append({cond_name: Config.tr("Flike:"),
                         value: model_night.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_night.getdata(day, "temp")) != "N/A")
                temperature.text =  model_night.getdata(day, "temp") + '°'
	    else{
                if ((model_night.getdata(day, "temp_low")) != "N/A")
                    temperature.text =  model_night.getdata(day, "temp_low") + '°'
            }

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
    }

 
    function getColor(t)
    {
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
    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Rectangle {
            id: day_rect
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: 72
            color: "black"
            Rectangle {
                id: left_arrow
                width: 72
                height: 72
                anchors.top: parent.top
                anchors.left: parent.left
                color: "black"
                visible: day > 0 ? true : false;
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
                        if (day > 0){
                            console.log("prev day");
                            day--;
                            fullweather.updateperiod();
                        }
                    }
                }

            }
            Rectangle {
                 id: right_arrow
                 width: 72
                 height: 72
                 anchors.top: parent.top
                 anchors.right: parent.right
                 color: "black"
                 visible: day < (model_day.rowCount()-1) ? true : false;
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
                         if (!current && day < model_day.rowCount()-1){
                            console.log("next day");
                            day++;
                            fullweather.updateperiod();
                        }
                     }
                 }
             }
            Text {
                id: dayname
                anchors.top: parent.top
                anchors.left: parent.left
                height: parent.height
                width: parent.width
                text: (current) ? Config.tr("Today") : model_day.getdata(day, "date");
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "white"
                font.pointSize: 20
            }
        }

        Rectangle {
            id: current_rect
            anchors.top: day_rect.bottom
            width: parent.width
            height: 274
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
            Text {
                id: desc
                text: description_text 
                anchors.left: parent.left
                anchors.top: now.bottom
                width: current_rect.width
                height: 44
                color: "white"
                font.pointSize: 18
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
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
                anchors.topMargin: 20
                anchors.left: parent.left
                anchors.leftMargin: margin
                anchors.right: parent.right
                anchors.rightMargin: margin
                width: parent.width - 2*margin
                height: 245
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
 

        }

    }
}

