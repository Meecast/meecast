import Qt 4.7
/*
LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "128x128"
    maximumSize: "300x300"
    preferredSize: "256x256"
*/
Rectangle {
    width: 480
    height: 796
    color: "black"
    //color: getColor(-40)
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
                anchors.leftMargin: margin
                anchors.rightMargin: margin
                width: parent.width - 2*margin
                height: 72
                color: "black"


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

                    /*
                    onMovementStarted: {
                        console.log("movement started "+contentX);
                        if (currentIndex < count-1) currentIndex++;
                        else currentIndex = 0;
                        console.log("movement started "+contentX);

                    }
                    onMovementEnded: {
                        contentX = -160 + currentIndex*160
                        //positionViewAtIndex(currentIndex, ListView.Center)
                        console.log("movement ended "+contentX);
                    }*/
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

                }
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_title.png"}
                }
                /*
                Rectangle {
                    anchors.fill: parent
                    gradient: Gradient {
                        //rotation: 90
                        GradientStop {position: 0.0; color: "#FF000000"}
                        GradientStop {position: 0.5; color: "#00000000"}
                        GradientStop {position: 1.0; color: "#FF000000"}
                    }
                }*/
            }

            Rectangle {
                anchors.top: station_rect.bottom
                width: parent.width
                height: 274
                color: getColor(Current.temperature_high)
                Loader {
                    anchors.fill: parent
                    sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
                }
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
                    source: Config.iconspath + "/" + Config.iconset + "/" + Current.icon
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
                    text: Current.temperature_high + '°'
                    font.pointSize: 24
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    id: desc
                    text: Current.description
                    anchors.left: parent.left
                    anchors.top: now.bottom
                    width: parent.width
                    height: 44
                    color: "white"
                    font.pointSize: 18
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
                    text: Current.humidity+"%"
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
                    text: Current.wind_direction
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
                    text: Current.pressure
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
                    text: Current.wind_speed
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
}
