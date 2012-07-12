import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0


Page {
    id: standbysettingspage
    property int margin: 16
    property string color_font_stationname: Config.standby_color_font_stationname
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                Config.set_standby_color_font_stationname(color_font_stationname)
                pageStack.pop();
            }
        }

    }

    orientationLock: PageOrientation.LockPortrait

    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: "#999999"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 274
            color: "black"
        }
    }

    Rectangle {
        id: title_rect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 40
        color: "black"
        Label {
            id: title
            anchors.fill: parent
            color: "white"
//            text: Config.tr("Select the color of font for station name")
            text: Config.tr("Settings for standby widget")
            //font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: title_station_color_font
        anchors.top: title_rect.bottom
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 40
        color: "black"
        Label {
            id: title_station_name
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select the color of font for station name")
            //font.family: "Nokia Pure Light"
            font.pixelSize: 20
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: page
        //width: 500;
        height: 60
     
        anchors.top: title_station_color_font.bottom
        anchors.left: parent.left
        anchors.topMargin: margin 
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "black"

        Grid {
            id: colorPicker
            x: 4; anchors.bottom: page.bottom; anchors.bottomMargin: 4
            rows: 1; columns: 8; spacing: 5

            ColorCell { cellColor: "red"; if (color_font_stationname == "#ff0000") {selected = true};  onClicked: {color_font_stationname = "#ff0000"; cellColor = "white"}}
            ColorCell { cellColor: "white"; onClicked:{color_font_stationname = "#ffffff";}}
            ColorCell { cellColor: "cyan"; onClicked: helloText.color = cellColor }
            ColorCell { cellColor: "magenta"; onClicked: helloText.color = cellColor }
            ColorCell { cellColor: "green"; onClicked: helloText.color = cellColor }
            ColorCell { cellColor: "blue"; onClicked: helloText.color = cellColor }
            ColorCell { cellColor: "yellow"; onClicked: helloText.color = cellColor }
            ColorCell { cellColor: "black"; onClicked: console.log (color_font_stationname) }
        }
 }



}


