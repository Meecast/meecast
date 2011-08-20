//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: fullweather
    property int margin: 16
    property int day: 0
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
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

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                id: item2
                text: qsTr("About")
                onClicked: {
                    main.openFile("AboutPage.qml");
                }
            }
        }
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
            c2 = (t + 15)*(66/255-216/255)/(-30+15) + 216/255;
            return Qt.rgba(0, c2, 1, 1);
        }else if (t < -30){
            c1 = (t + 30)*(132/255-0)/(-30+15) + 0;
            c2 = (t + 30)*(0-66/255)/(-30+15) + 66/255;
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
            Text {
                id: dayname
                anchors.top: parent.top
                anchors.left: parent.left
                height: parent.height
                width: parent.width
                text: Forecast_model.getdata(day, "date");
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
            color: getColor(Current.temperature_high)
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
                text: "Day"
                font.pointSize: 24
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Image {
                id: icon
                source: Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getdata(day, "pict")
                width: 128
                height: 128
                anchors.top: parent.top
                anchors.topMargin: -22
                anchors.left: now.right
            }
            Text {
                anchors.top: parent.top
                anchors.left: icon.right
                anchors.rightMargin: margin
                width: 160
                height: 84
                color: "white"
                text: Forecast_model.getdata(day, "temp_high") + '°'
                font.pointSize: 24
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                Component.onCompleted: {
                    current_rect.color = getColor(Forecast_model.getdata(day, "temp_high"));
                }
            }
            Text {
                id: desc
                text: Forecast_model.getdata(day, "description")
                anchors.left: parent.left
                anchors.top: now.bottom
                width: current_rect.width
                height: 44
                color: "white"
                font.pointSize: 16
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            ListModel {
                id: condition
            }
            Component.onCompleted: {
                if ((Forecast_model.getdata(day, "humidity")) != "N/A")
                    condition.append({cond_name: qsTr("Humidity"),
                                 value: Forecast_model.getdata(day, "humidity")+'%'});
                if ((Forecast_model.getdata(day, "wind_direction")) != "")
                    condition.append({cond_name: qsTr("Wind direction:"),
                                 value: Forecast_model.getdata(day, "wind_direction")});
                if ((Forecast_model.getdata(day, "pressure")) != "N/A")
                    condition.append({cond_name: qsTr("Pressure:"),
                                 value: Forecast_model.getdata(day, "pressure")});
                if ((Forecast_model.getdata(day, "wind_speed")) != "N/A")
                    condition.append({cond_name: qsTr("Wind speed:"),
                                 value: Forecast_model.getdata(day, "wind_speed") + ' ' + Config.windspeedunit});

                if ((Forecast_model.getdata(day, "ppcp")) != "N/A")
                    condition.append({cond_name: qsTr("Ppcp:"),
                                 value: Forecast_model.getdata(day, "ppcp")});
                if ((Forecast_model.getdata(day, "wind_gust")) != "N/A")
                    condition.append({cond_name: qsTr("Wind gust:"),
                                 value: Forecast_model.getdata(day, "wind_gust") + ' ' + Config.windspeedunit});
                if ((Forecast_model.getdata(day, "flike")) != "N/A")
                    condition.append({cond_name: qsTr("Flike:"),
                                 value: Forecast_model.getdata(day, "flike") + ' ' + Config.temperatureunit});
            }
            GridView {
                id: grid
                anchors.top: desc.bottom
                anchors.topMargin: 32
                anchors.left: parent.left
                anchors.leftMargin: margin
                anchors.right: parent.right
                anchors.rightMargin: margin
                width: parent.width - 2*margin
                height: 250
                cellWidth: (parent.width - 2*margin) / 2
                model: condition
                delegate: Column {
                    width: grid.width / 2
                    spacing: 3
                    Text {
                        text: model.cond_name
                        color: "#999999"
                        font.pointSize: 16
                    }
                    Text {
                        text: model.value
                        color: "white"
                        font.pointSize: 16
                    }
                }
            }
            Rectangle {
		id: splitter
		color: "#303030"
		x: 20; width: parent.width - 40; height: 2
		anchors.top: grid.bottom 
		anchors.leftMargin: 20
		//anchors.topMargin: 30 
	    }

        }

    }
}

