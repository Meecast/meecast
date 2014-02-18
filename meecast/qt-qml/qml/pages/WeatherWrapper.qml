import QtQuick 2.0
import Sailfish.Silica 1.0
import "./"

Page {
    id: main
    property int margin: 16
    property int screen_height : 960 
    property int screen_width : 540 
    property int real_current_id : Config._current_station_id() 
    property bool isUpdate: false
    property real flick_start_position: 0
    property bool menuitemgps: Config.gps

    objectName: "WeatherPage"

    ListModel {
        id: listModel
        function update() {
            clear()
            for (var i=0; i<Config.stations().length; i++) {
                append({"id": i, "key": Config.stations()[i]})
            }
        }
        Component.onCompleted: update("")
    }
    ListModel {
        id: current_stub 
        ListElement { description: "" }
    }
    ListModel {
        id: forecast_stub 
        function update() {
            for (var i=0; i<21; i++) {
                    append({"id": i})
            }
        }
        Component.onCompleted: update()
    }

    function openFile(file){
        pageStack.push(Qt.resolvedUrl(file))
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
            //console.log(t+ " "+c2);
            return Qt.rgba(0, c2, 1, 1);
        }else if (t < -30){
            c1 = (t - (-30))*(132/255-0)/(-30+15) + 0;
            c2 = (t - (-30))*(0-66/255)/(-30+15) + 66/255;
            return Qt.rgba(c1, c2, 1, 1);
        }

    }
    function getAngle(s){
        var a;

        /*
        switch (s[0]){
        case 'N':
            a = 0;
            break;
        case 'S':
            a = 8;
            break;
        case 'E':
            a = 4;
            break;
        case 'W':
            a = -4;
            break;
        }
        if (a.length == 1) return a*22.5;*/
        switch (s){
        case 'S':
            return 0;
        case 'SSW':
            return 22.5;
        case 'SW':
            return 45;
        case 'WSW':
            return (45+22.5);
        case 'W':
            return 90;
        case 'WNW':
            return (90+22.5);
        case 'NW':
            return (90+45);
        case 'NNW':
            return (180-22.5);
        case 'N':
            return 180;
        case 'NNE':
            return (180+22.5);
        case 'NE':
            return (180+45);
        case 'ENE':
            return (270-22.5);
        case 'E':
            return 270;
        case 'ESE':
            return (270+22.5);
        case 'SE':
            return (270+45);
        case 'SSE':
            return (360-22.5);

        }

    }

    function update(){
        console.log("start update");
        isUpdate = true;
        Config.updatestations();
    }

    function current_model(name){
        return Current.getdata(0, name);
    }
    function get_count_days(i){
        console.log( "get_count_days()")
        /* For pretty view but it works very slowly  and Cover doesn't work correctly*/
        //Config.station_by_index(i);
        //main.updatemodels();
        /* To do make right forecast model */
        return Forecast_model.rowCount()
    }

    function forecast_model(i, name){
//        console.log("forecast_model(i, name)");
//        console.log(i);
        return Forecast_model.getdata(i, name);
    }

    function updatemodels(){
        console.log("QML updatemodels())")
        Current.reload_data(Config.filename);
        Current.update_model(0);
        Current_night.update_model(1);
        Forecast_model.update_model(2);
        Forecast_night_model.update_model(3);
        Forecast_hours_model.update_model(4);
        //forecast_stub.update()
        //list.height = 80 * Forecast_model.rowCount();
//        dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
//        current_rect.visible = Current.rowCount() == 0 ? false : true;
        //list.visible = (Forecast_model.rowCount() == 0) ? false : true;
        listview.visible = Config.stationname == "Unknown" ? false : true
        startview.visible = Config.stationname == "Unknown" ? true : false
//        startview.model = stations 
    }

    function stationname1_index(i){
        return Config.stationname_index(i)
    }

    Connections {
        target: Config
        onConfigChanged: {
            console.log("end update station name = "+Config.stationname);
            startview.visible = Config.stationname == "Unknown" ? true : false;
            listview.visible = Config.stationname == "Unknown" ? false : true;
            listModel.update()
            updatemodels();
            isUpdate = false;

        }
    }

    Item{
        Timer{
            interval: 500; running: true; repeat: false
            onTriggered: { 
                listview.positionViewAtIndex(real_current_id, ListView.Beginning)
                Config._current_station_id(main.real_current_id)
                Config.saveConfig()
            }
        }
    }

    SilicaListView {
        id: listview
        anchors.fill: parent
        height: 960 
        width: 540
        visible: Config.stationname == "Unknown" ? false : true
        orientation: Qt.Horizontal
        highlightRangeMode: ListView.StrictlyEnforceRange
        snapMode: ListView.SnapOneItem
        model: listModel
        interactive: true
        currentIndex: Config._current_station_id() 
        Component.onCompleted: {}
        onCurrentIndexChanged:{ }
        onFlickEnded: { 
            //console.log("onFlickEnded")
            real_current_id = indexAt(contentX,contentY);
            Config._current_station_id(main.real_current_id)
            Config.saveConfig();
            Config.refreshconfig();
        }
	delegate: WeatherStationDelegate { id: weatherStationDelegate } 
     }
     Rectangle {
        id: startview
        visible: Config.stationname == "Unknown" ? true : false
        width: screen_width
        height: screen_height
        color: "black"
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: 72
            color: "black"
        }
        Text {
            id: empty_text
            anchors.left: parent.left
            anchors.top: parent.top
            width: parent.width
            height: 72
            color: "white"
            font.pointSize: 28
            text: Config.tr("MeeCast")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Loader {
            id: empty_background1
            anchors.top: empty_text.bottom
            anchors.left: parent.left
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
        }
        Rectangle {
            anchors.top: empty_background1.bottom
            width: parent.width
            height: 600 
            //height: dataview.height - 274
            color: "black"
        }
        Label {
            horizontalAlignment: Text.AlignHCenter
            text: Config.tr("No locations are set up yet.")
            font.pixelSize: 54 
            color: "#999999"
            wrapMode: Text.Wrap
            width: parent.width - 2*margin
            anchors.top: parent.top
            anchors.topMargin: 250
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: margin
            anchors.rightMargin: margin
        }
        Button {
            text: Config.tr("Set locations")
            onClicked: {
                main.openFile("SourcePage.qml")
            }
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 150
        }
    }
}

