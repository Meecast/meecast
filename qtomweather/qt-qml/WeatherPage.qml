//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: main
    property int margin: 16
    property bool isUpdate: false
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-refresh"
            onClicked: {
                main.update();
            }
        }
        Image {
            id: sourceicon
            source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
            smooth: true	
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
                id: item1
                text: qsTr("Settings")
                onClicked: {
                    main.openFile("StationsPage.qml");

                }
            }
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
    function update()
    {
        console.log("start update");
        isUpdate = true;
        Config.updatestations();
    }
    function updatemodels()
    {
        Current.update(Config.filename, true);
        Forecast_model.update(Config.filename, false);
        list.height = 80 * Forecast_model.rowCount();
        dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
        current_rect.visible = Current.rowCount() == 0 ? false : true;
        list.visible = (Forecast_model.rowCount() == 0) ? false : true;
    }
    function updatestationname()
    {
        main.updatemodels();
        stationname.text = Config.stationname;
        prevstationimage.visible = Config.prevstationname == "" ? false : true;
        nextstationimage.visible = Config.nextstationname == "" ? false : true;
        sourceicon.visible = false;
        sourceicon.source = Config.imagespath + "/" + Config.source + ".png";
        sourceicon.visible = true;
    }

    Connections {
        target: Config
        onConfigChanged: {
            console.log("end update station name = "+Config.stationname);
            startview.visible = Config.stationname == "Unknown" ? true : false;
            mainview.visible = Config.stationname == "Unknown" ? false : true;
            main.updatestationname();
            isUpdate = false;

        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        contentWidth: flickable.width
        contentHeight: station_rect.height + current_rect.height + list.height
        flickableDirection: Flickable.VerticalFlick
        //clip: true
        Item {
            id: startview
            visible: Config.stationname == "Unknown" ? true : false
            width: parent.width
            height: parent.height - 200
            Column {
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter

                spacing: 20
                Label {
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("No weather stations are set up.")
                    font.pixelSize: 40
                    color: "#303030"
                    wrapMode: Text.Wrap
                    width: parent.width
                    //anchors.horizontalCenter: parent.horizontalCenter
                }
                Button {
                    text: qsTr("Set them up")
                    onClicked: {
                        main.openFile("StationsPage.qml");
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }
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
                        text: qsTr("Updating forecast")
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
                height: 72
                color: "black"
                visible: isUpdate ? false : true
                Rectangle {
		    id: left_arrow
                    width: 72
                    height: 72
                    anchors.top: parent.top
                    anchors.left: parent.left
                    color: "black"
                    Image {
                        id: prevstationimage
                        source: Config.imagespath + "/arrow_left.png"
                        width:  62 
                        height: 62
                        anchors.top: parent.top
                        anchors.left: parent.left
                        //anchors.leftMargin: margin
                        visible: Config.prevstationname == "" ? false : true;
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

               Rectangle {
		    id: right_arrow
                    width: 72
                    height: 72
                    anchors.top: parent.top
                    anchors.right: parent.right
                    color: "black"
                    Image {
                        id: nextstationimage
                        source: Config.imagespath + "/arrow_right.png"
                        width: 62 
                        height: 62
                        anchors.top: parent.top
                        anchors.right: parent.right
                        //anchors.verticalCenter: parent.verticalCenter
                        //anchors.rightMargin: margin
                        visible: Config.nextstationname == "" ? false : true;
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
                Text {
                    id: stationname
                    anchors.top: parent.top
                    anchors.left: left_arrow.right
                    anchors.right: right_arrow.left
                    height: parent.height
                    width: parent.width - right_arrow.width - left_arrow.width
                    text: Config.stationname
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    font.pointSize: 20

                }
 
                /*
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_title.png"}
                }*/
            }
            Item {
                id: dataview
                visible: (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false
                anchors.top: station_rect.bottom
                width: parent.width
                height: parent.height - 200
                Column {
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter

                    spacing: 20
                    Label {
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Looks like there is no info for this day. Update usually helps.")
                        font.pixelSize: 40
                        color: "#303030"
                        wrapMode: Text.Wrap
                        width: parent.width
                        //anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Button {
                        text: qsTr("Update")
                        onClicked: {
                            main.update();
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
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
                            text: model.current == true ? "Now" : "Today"
                            font.pointSize: 24
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
                            width: 160
                            height: 84
                            color: "white"
                            text: model.temp_high + '°'
                            font.pointSize: 24
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            Component.onCompleted: {
                                current_rect.color = getColor(temp_high);
                            }
                        }
                        Text {
                            id: desc
                            text: model.description
                            anchors.left: parent.left
                            anchors.top: now.bottom
                            width: current_rect.width
                            height: 44
                            color: "white"
                            font.pointSize: 16
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
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
                            font.pointSize: 16
                            verticalAlignment: Text.AlignVCenter
                        }
                        Image {
                            id: wind_direction
                            source: Config.imagespath + "/wind_direction.png"
                            anchors.top: desc.bottom
                            anchors.topMargin: 32
                            anchors.left: parent.left
                            anchors.leftMargin: margin+224
                            width: 30
                            height: 30
			    smooth: true
                        }
                        Text {
                            text: model.wind_direction
                            anchors.left: wind_direction.right
                            anchors.leftMargin: 8
                            anchors.top: desc.bottom
                            anchors.topMargin: 32
                            height: 30
                            color: "white"
                            font.pointSize: 16
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
                            text: model.pressure + " mbar"
                            anchors.left: pressure.right
                            anchors.leftMargin: 8
                            anchors.top: humidity.bottom
                            anchors.topMargin: 22
                            height: 30
                            color: "white"
                            font.pointSize: 16
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
                            text: model.wind_speed + ' ' + Config.windspeedunit
                            anchors.left: wind_speed.right
                            anchors.leftMargin: 8
                            anchors.top: humidity.bottom
                            anchors.topMargin: 22
                            height: 30
                            color: "white"
                            font.pointSize: 16
                            verticalAlignment: Text.AlignVCenter
                        }
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
                            text: model.date
                            color: "white"
                            font.pointSize: 16
                            anchors.left: parent.left
                            anchors.leftMargin: margin
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
                            font.pointSize: 16
                            color: getColor(temp_high)
                            text: model.temp_high + '°'
                            anchors.right: parent.right
                            anchors.rightMargin: margin + 70
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            id: txt_templo
                            font.pointSize: 16
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
                                pageStack.push(Qt.resolvedUrl("FullWeatherPage.qml"),
                                               {day: index, day_period: "day"}
					       )

                            }
                            hoverEnabled: true

                        }

                    }
                }
            } //component itemDelegate

        }

    }

}
