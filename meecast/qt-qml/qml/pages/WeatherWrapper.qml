import QtQuick 2.0
import Sailfish.Silica 1.0
import QtQml.Models 2.1
import "./"

Page {
    id: main
    property int margin: 16
    property int screen_height : 960 
    property int screen_width : 540 
    property bool isUpdate: false
    property real flick_start_position: 0
    property bool menuitemgps: Config.gps
 
   objectName: "WeatherPage"
    /*
    ListModel {
        id: listModel
        function update(my_text) {
            clear()
            for (var i=0; i<country_model.rowCount(); i++) {
                if (my_text == "" || country_model.get(i).name.indexOf(my_text) >= 0) {
                    append({"name": country_model.get(i).name, "key": country_model.get(i).key})
                }
            }
        }
        Component.onCompleted: update("")
    }
    */
    function openFile(file)
    {
        pageStack.push(Qt.resolvedUrl(file))
//        var component = Qt.createComponent(file);
//        if (component.status == Component.Ready){
//            pageStack.push(component);
//        }else {
//            console.log("error open file "+file);
//        }
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
            //console.log(t+ " "+c2);
            return Qt.rgba(0, c2, 1, 1);
        }else if (t < -30){
            c1 = (t - (-30))*(132/255-0)/(-30+15) + 0;
            c2 = (t - (-30))*(0-66/255)/(-30+15) + 66/255;
            return Qt.rgba(c1, c2, 1, 1);
        }

    }
    function getAngle(s)
    {
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

    function update()
    {
        console.log("start update");
        isUpdate = true;
        Config.updatestations();
    }

    function updatemodels()
    {
        Current.reload_data(Config.filename);
        Current.update_model(0);
        Current_night.update_model(1);
        Forecast_model.update_model(2);
        Forecast_night_model.update_model(3);
        Forecast_hours_model.update_model(4);
        list.height = 80 * Forecast_model.rowCount();
        dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
        current_rect.visible = Current.rowCount() == 0 ? false : true;
        list.visible = (Forecast_model.rowCount() == 0) ? false : true;
    }

    function updatestationname()
    {
        main.updatemodels();
        stationname.title = Config.stationname;
       // left_arrow.visible = Config.prevstationname == "" ? false : true;
      //  right_arrow.visible = Config.nextstationname == "" ? false : true;
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

    DelegateModel { 
        id: stations
        model: Config.stations()
        delegate: WeatherStationDelegate { id: weatherStationDelegate }
    } 

    SilicaListView {
        id: listView
        anchors.fill: parent
        orientation: Qt.Horizontal
        highlightRangeMode: ListView.StrictlyEnforceRange
        snapMode: ListView.SnapOneItem
        model: stations
        interactive: true
        currentIndex: 1

     }
}
