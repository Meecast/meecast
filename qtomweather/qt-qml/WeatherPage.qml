//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: main
    property int margin: 16
    tools: ToolBarLayout {
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
                text: "Update"
                onClicked: {
                    // update
                    main.update();
                }
            }
            MenuItem {
                id: item2
                text: "Settings"
                onClicked: {
                    // open settings
                    console.log("open settings");
                    main.openFile("SettingsPage.qml");
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
        Config.updatestations();
        main.updatemodels();

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

    Connections {
        target: Config
        onConfigChanged: {
            console.log("qqqqqqqqqqqqqqqqqqqq "+Config.stationname);
            //prevstationname = Config.prevstationname;
            //stationname = Config.stationname;
            //nextstationname = Config.nextstationname;
            startview.visible = Config.stationname == "Unknown" ? true : false;
            mainview.visible = Config.stationname == "Unknown" ? false : true;
        }
    }

    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
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
                        main.openFile("SettingsPage.qml");
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
                id: station_rect
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: 72
                color: "black"

                Text {
                    id: prevstationname
                    anchors.top: parent.top
                    anchors.left: parent.left
                    height: parent.height
                    width: parent.width / 3
                    text: Config.prevstationname
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    font.pointSize: 20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (prevstationname.text != ""){
                                console.log("prev station");
                                Config.prevstation();
                                main.updatemodels();
                                stationname.text = Config.stationname;
                                prevstationname.text = Config.prevstationname;
                                nextstationname.text = Config.nextstationname;
                            }
                        }
                    }
                }

                Text {
                    id: stationname
                    anchors.top: parent.top
                    anchors.left: prevstationname.right
                    height: parent.height
                    width: parent.width / 3
                    text: Config.stationname
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    font.pointSize: 20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("refresh ");
                        }
                    }
                }
                Text {
                    id: nextstationname
                    anchors.top: parent.top
                    //anchors.left: stationname.rigth
                    anchors.right: parent.right
                    height: parent.height
                    width: parent.width / 3
                    text: Config.nextstationname
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "white"
                    font.pointSize: 20
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (nextstationname.text != ""){
                                console.log("next station");
                                Config.nextstation();
                                main.updatemodels();
                                stationname.text = Config.stationname;
                                prevstationname.text = Config.prevstationname;
                                nextstationname.text = Config.nextstationname;
                            }
                        }
                    }
                }
                /*
                ListView {
                    id: station
                    anchors.fill: parent
                    //width: parent.width
                    orientation: ListView.Horizontal
                    model: ["Moscow", "Vitebsk", "San Francisco", "Berlin"]

                    clip: true
                    interactive: false
                    //property int item_width: parent.width / 3 + 10
                    property int item_width: 160

                    onCurrentIndexChanged: {
                        console.log("current index = "+currentIndex+" item_width="+item_width);
                        contentX = -item_width + currentIndex*item_width
                        //contentX = -160

                        console.log("current index changed contentX="+contentX);
                    }
                    delegate: Item {
                        height: 72
                        width: station.item_width
                        Text {
                            //anchors.fill: parent
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            //anchors.centerIn: parent.Center
                            text: modelData
                            color: "white"
                            //color: ListView.isCurrentItem ? "red" : "white"
                            font.pointSize: 20
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                station.currentIndex = index;
                                //station.positionViewAtIndex(index, ListView.Center);
                                console.log("on click "+index);
                            }
                        }

                    }
                    Component.onCompleted: {
                        if (count > 1) currentIndex = 1;
                        else if (count == 1) currentIndex = 0;
                        else if (count == 0) currentIdex = -1;
                        //station.currentIndex = 0;
                        //station.contentX = -160;
                        //console.log("on complete contentx = "+station.contentX);
                    }

                }*/
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_title.png"}
                }
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
                    sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
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
                            text: "Now"
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
                        }
                        Text {
                            text: model.pressure
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
                                main.openFile("FullWeatherPage.qml");
                                //FullWeatherPage.station = "dfsfs";
                            }
                            hoverEnabled: true

                        }

                    }
                }
            } //component itemDelegate

        }

    }

}
