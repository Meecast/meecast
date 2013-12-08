import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: standbysettingspage
    property int margin: 16
    property string color_font_stationname: Config.standby_color_font_stationname
    property string color_font_temperature: Config.standby_color_font_temperature
    property string color_font_current_temperature: Config.standby_color_font_current_temperature
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                Config.set_standby_color_font_stationname(color_font_stationname)
                Config.set_standby_color_font_temperature(color_font_temperature)
                Config.set_standby_color_font_current_temperature(color_font_current_temperature)
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
    function resetTempGird(){
        red_cellt.unselected();
        green_cellt.unselected();
        magenta_cellt.unselected();
        cyan_cellt.unselected();
        blue_cellt.unselected();
        black_cellt.unselected();
        white_cellt.unselected();
        yellow_cellt.unselected();
    }
    function resetTempCurrentGird(){
        red_celltc.unselected();
        green_celltc.unselected();
        magenta_celltc.unselected();
        cyan_celltc.unselected();
        blue_celltc.unselected();
        black_celltc.unselected();
        white_celltc.unselected();
        yellow_celltc.unselected();
    }

    orientationLock: PageOrientation.LockPortrait

    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 120
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
        height: 70
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
        height: 30
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
        id: box_temperature_color_font 
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
                        selected : color_font_temperature == "#ff0000" ? true : false;   
                        onClicked: { resetTempGird(); color_font_temperature = "#ff0000"; }}
            ColorCell { cellColor: "white"; 
                        id: white_cellt;
                        selected : color_font_temperature == "#ffffff" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#ffffff";  }}
            ColorCell { cellColor: "cyan";
                        id: cyan_cellt;
                        selected : color_font_temperature == "#00ffff" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#00ffff"; }}
        
            ColorCell { cellColor: "magenta";
                        id: magenta_cellt;
                        selected : color_font_temperature == "#ff00ff" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#ff00ff"; }}
            ColorCell { cellColor: "green";
                        id: green_cellt;
                        selected : color_font_temperature == "#00ff00" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#00ff00"; }}
            ColorCell { cellColor: "blue";
                        id: blue_cellt;
                        selected : color_font_temperature == "#0000ff" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#0000ff"; }}
            ColorCell { cellColor: "yellow"; 
                        id: yellow_cellt;
                        selected : color_font_temperature == "#ffff00" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#ffff00"; }}
            ColorCell { cellColor: "black";
                        id: black_cellt;
                        selected : color_font_temperature == "#000000" ? true : false; 
                        onClicked:{ resetTempGird(); color_font_temperature = "#000000"; }}
        }
 }
    Rectangle {
        id: title_current_temperature_color_font
        anchors.top: box_temperature_color_font.bottom
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 30
        color: "black"
        Label {
            id: title_current_temperature_color
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select the color of font for current temperature")
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
     
        anchors.top: title_current_temperature_color_font.bottom
        anchors.left: parent.left
        anchors.topMargin: margin 
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "black"

        Grid {
            id: colorPickertc
            x: 4; anchors.bottom: page.bottom; anchors.bottomMargin: 4
            rows: 1; columns: 8; spacing: 5

            ColorCell { cellColor: "red"; 
                        id: red_celltc;
                        selected : color_font_current_temperature == "#ff0000" ? true : false;   
                        onClicked: { resetTempCurrentGird(); color_font_current_temperature = "#ff0000"; }}
            ColorCell { cellColor: "white"; 
                        id: white_celltc;
                        selected : color_font_current_temperature == "#ffffff" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#ffffff";  }}
            ColorCell { cellColor: "cyan";
                        id: cyan_celltc;
                        selected : color_font_current_temperature == "#00ffff" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#00ffff"; }}
        
            ColorCell { cellColor: "magenta";
                        id: magenta_celltc;
                        selected : color_font_current_temperature == "#ff00ff" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#ff00ff"; }}
            ColorCell { cellColor: "green";
                        id: green_celltc;
                        selected : color_font_current_temperature == "#00ff00" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#00ff00"; }}
            ColorCell { cellColor: "blue";
                        id: blue_celltc;
                        selected : color_font_current_temperature == "#0000ff" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#0000ff"; }}
            ColorCell { cellColor: "yellow"; 
                        id: yellow_celltc;
                        selected : color_font_current_temperature == "#ffff00" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#ffff00"; }}
            ColorCell { cellColor: "black";
                        id: black_celltc;
                        selected : color_font_current_temperature == "#000000" ? true : false; 
                        onClicked:{ resetTempCurrentGird(); color_font_current_temperature = "#000000"; }}
        }
 }



}


