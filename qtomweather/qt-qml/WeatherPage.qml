import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: main
    tools: ToolBarLayout {
        ToolIcon {
            platformStyle: ToolItemStyle {inverted: true}
            iconId: "toolbar-view-menu"
            onClicked: menu.open();
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
            console.log("error open file");
        }
    }

    Menu {
        id: menu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                id: item1
                text: "Update"
                onClicked: {
                    // update
                    console.log("refresh ");
                    Config.updatestations();
                    console.log("update done");
                    Current.update(Config.filename, true);
                    Forecast_model.update(Config.filename, false);
                    //menu.accept();
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
    property int margin: 16

    function getColor(t)
    {
        var c1, c2, c3;
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

    Connections {
        target: Config
        onConfigChanged: {
            console.log("qqqqqqqqqqqqqqqqqqqq");
        }
    }

    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        Item {
            anchors.fill: parent
            height: 900
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
                                Forecast_model.update(Config.filename, false);
                                Current.update(Config.filename, true);
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
                                Forecast_model.update(Config.filename, false);
                                Current.update(Config.filename, true);
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

            Rectangle {
                id: current_rect
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
                            text: date
                            color: "white"
                            font.pointSize: 16
                            anchors.left: parent.left
                            anchors.leftMargin: margin
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Image {
                            source: Config.iconspath + "/" + Config.iconset + "/" + pict
                            width: 64
                            height: 64
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Text {
                            id: txt_temphi
                            font.pointSize: 16
                            color: getColor(temp_high)
                            text: temp_high + '°'
                            anchors.right: parent.right
                            anchors.rightMargin: margin + 70
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            id: txt_templo
                            font.pointSize: 16
                            color: "#889397"
                            text: temp_low + '°'
                            anchors.right: parent.right
                            anchors.rightMargin: margin
                            height:parent.height
                            verticalAlignment: Text.AlignVCenter
                        }


                        /*
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                uiloader.source = "Details.qml";
                                //list.visible = false;
                                columnlist.visible = false
                                uiloader.item.item_id = id_item;
                                //uiloader.item.width = frontItemOmweather.width
                                //uiloader.item.height = frontItemOmweather.height
                            }
                            hoverEnabled: true

                        }*/

                    }
                }
            } //component itemDelegate

        }

    }

}
