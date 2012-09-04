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

    function resetGird(){
        red_cell.unselected();
        green_cell.unselected();
        magenta_cell.unselected();
        cyan_cell.unselected();
        blue_cell.unselected();
        black_cell.unselected();
        white_cell.unselected();
        yellow_cell.unselected();
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
        id: box_station_color
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

            ColorCell { cellColor: "red"; 
                        id: red_cell;
                        selected : color_font_stationname == "#ff0000" ? true : false;   
                        onClicked: { resetGird(); color_font_stationname = "#ff0000"; }}
            ColorCell { cellColor: "white"; 
                        id: white_cell;
                        selected : color_font_stationname == "#ffffff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ffffff";  }}
            ColorCell { cellColor: "cyan";
                        id: cyan_cell;
                        selected : color_font_stationname == "#00ffff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#00ffff"; }}
        
            ColorCell { cellColor: "magenta";
                        id: magenta_cell;
                        selected : color_font_stationname == "#ff00ff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ff00ff"; }}
            ColorCell { cellColor: "green";
                        id: green_cell;
                        selected : color_font_stationname == "#00ff00" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#00ff00"; }}
            ColorCell { cellColor: "blue";
                        id: blue_cell;
                        selected : color_font_stationname == "#0000ff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#0000ff"; }}
            ColorCell { cellColor: "yellow"; 
                        id: yellow_cell;
                        selected : color_font_stationname == "#ffff00" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ffff00"; }}
            ColorCell { cellColor: "black";
                        id: black_cell;
                        selected : color_font_stationname == "#000000" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#000000"; }}
        }
 }
    Rectangle {
        id: title_temperature_color_font
        anchors.top: box_station_color.bottom
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 40
        color: "black"
        Label {
            id: title_temperature_color
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select the color of font for temperature")
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
     
        anchors.top: title_temperature_color_font.bottom
        anchors.left: parent.left
        anchors.topMargin: margin 
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "black"

        Grid {
            id: colorPickert
            x: 4; anchors.bottom: page.bottom; anchors.bottomMargin: 4
            rows: 1; columns: 8; spacing: 5

            ColorCell { cellColor: "red"; 
                        id: red_cellt;
                        selected : color_font_stationname == "#ff0000" ? true : false;   
                        onClicked: { resetGird(); color_font_stationname = "#ff0000"; }}
            ColorCell { cellColor: "white"; 
                        id: white_cellt;
                        selected : color_font_stationname == "#ffffff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ffffff";  }}
            ColorCell { cellColor: "cyan";
                        id: cyan_cellt;
                        selected : color_font_stationname == "#00ffff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#00ffff"; }}
        
            ColorCell { cellColor: "magenta";
                        id: magenta_cellt;
                        selected : color_font_stationname == "#ff00ff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ff00ff"; }}
            ColorCell { cellColor: "green";
                        id: green_cellt;
                        selected : color_font_stationname == "#00ff00" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#00ff00"; }}
            ColorCell { cellColor: "blue";
                        id: blue_cellt;
                        selected : color_font_stationname == "#0000ff" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#0000ff"; }}
            ColorCell { cellColor: "yellow"; 
                        id: yellow_cellt;
                        selected : color_font_stationname == "#ffff00" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#ffff00"; }}
            ColorCell { cellColor: "black";
                        id: black_cellt;
                        selected : color_font_stationname == "#000000" ? true : false; 
                        onClicked:{ resetGird(); color_font_stationname = "#000000"; }}
        }
 }



}


