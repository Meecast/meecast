import QtQuick 2.12
import "./"
import QtQuick.Controls 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
//import Sailfish.Silica 1.0
//import Nemo.Notifications 1.0
import "./"

Page {
    id: main
    property int margin: 16
    property int screen_height : main.height 
    property int screen_width : main.width 
    //property real ratio: Theme.pixelRatio 
    property real ratio: Screen.devicePixelRatio

//    property real ratio: 0.5
    //property real ratio: main.screen_width/540
    property int column_rect_height: 72*ratio
    property int large_FontPointSize: 28*ratio
    property int top_rect_height: 274*ratio
    property int huge_PixelSize: 54*ratio
    property int real_current_id : Config._current_station_id() 
    property bool isUpdate: false
    property real flick_start_position: 0
    property bool menuitemgps: Config.gps
    property bool flipmoving: false
    property string station_name_text: ""


    objectName: "WeatherPage"


    StackView.onActivated: pageStack.currentItem.forceActiveFocus()
    header: RowLayout {
        id: headerRowLayout
        Label {
          text: station_name_text
          Layout.fillWidth: true
          horizontalAlignment: Text.AlignHCenter
          color: Theme.primaryColor
          font.pointSize: large_FontPointSize
          MouseArea {
             anchors.fill: parent
             onClicked: {
                 console.log("111111111111111")
             }
          }
        }
    }

    ListModel {
        id: listModel
        function update() {
          //  console.log("listModel.update()")
            clear()
            for (var i=0; i<Config.stations().length; i++) {
                append({"id": i, "key": Config.stations()[i]})
            //    console.log("append ", i)
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
    
    function update_models (station_index){
        /* console.log("update_models ", station_index); */
    	Config._current_station_id(station_index)
        Current.reload_data(Config.data_filename);
        Current.update_model(0);
        Forecast_model.update_model(2);
        /* hack */
        forecast_stub.append({"id": 0})
        startview.visible = Config.stationname == "Unknown" ? true : false;
        listview.visible = Config.stationname == "Unknown" ? false : true;
    }
    function current_model_count(station_index){
        console.log("current_model_count ", station_index);
        if (station_index >= 0 && station_index != Config._current_station_id()){
            update_models(station_index)
        }
        return Current.rowCount();
    }

    function forecast_model_count(station_index){
        /* console.log("forecast_model_count ", station_index); */
        if (station_index >= 0 && station_index != Config._current_station_id()){
            update_models(station_index)
        }
        return Forecast_model.rowCount();
    }

    function current_model(name, station_index){
        /* console.log("current_model ", station_index); */
        if (station_index >= 0 && station_index != Config._current_station_id()){
            update_models(station_index)
        }
        //console.log("Current.getdata(0, name)", Current.getdata(0, name))
        return Current.getdata(0, name);
    }

    function forecast_model(i, name, station_index){
        if (station_index >= 0 && station_index != Config._current_station_id()){
            update_models(station_index)
        }
        // console.log("Forecast_model.getdata(i, name)", i, " ", name," ", station_index, " ", Forecast_model.getdata(i, name))
        return Forecast_model.getdata(i, name);
    }

    function updatemodels(){
        console.log("QML updatemodels())")
        Current.reload_data(Config.data_filename);
        Current.update_model(0);
        Forecast_model.update_model(2);
        listview.visible = Config.stationname == "Unknown" ? false : true
        startview.visible = Config.stationname == "Unknown" ? true : false
    }

    function stationname1_index(i){
        return Config.stationname_index(i)
    }

    /*
    Notification {
         id: notification
         summary: Config.tr("Error")
         body: ""
    }
    */
    Connections {
        target: Config
        onConfigChanged: {
            console.log("WeatherWrapper.qml onConfigChanged() station - "+ Config.stationname);
            startview.visible = Config.stationname == "Unknown" ? true : false;
            listview.visible = Config.stationname == "Unknown" ? false : true;
            listModel.update()
            updatemodels();
            isUpdate = false;
            listview.positionViewAtIndex(Config._current_station_id(), ListView.Beginning)
        }
        onConfigChangeStationOnMainPage: {
            console.log("WeatherWrapper.qml onConfigChangeStationOnMainPage ", Config._current_station_id());
            listview.positionViewAtIndex(Config._current_station_id(), ListView.Beginning);
        }
        onErrorInConfig: {
            console.log("onErrorInConfig")
            console.log(text)
            notification.body = Config.tr(text)
            notification.publish()
        }
    }

    /*
     * I don't know what this code does  :( *
    Item{
        Timer{
            interval: 700; running: true; repeat: false
            onTriggered: { 
                listview.positionViewAtIndex(real_current_id, ListView.Beginning)
                Config._current_station_id(main.real_current_id)
                Config.saveConfig()
                Config.refreshconfig2();
            }
        }
    }
    */
    Flickable {
     //SilicaFlickable {
        anchors.fill: parent
        contentHeight: screen_height
        id: startview
        Drawer {
            id: drawer
            width: main_window.width
            height: drawer_layout.height * 1.2
            edge: Qt.TopEdge
            dragMargin : Qt.styleHints.startDragDistance*2
            ColumnLayout {
                spacing: 20
                id: drawer_layout
                width: main_window.width
                Rectangle{
                    height: drawer_layout.spacing *1.5
                }
                Button {
                    id: button_about
                    Layout.fillWidth: true
                    text: Config.tr("About")
                    font.pointSize: large_FontPointSize 
                    MouseArea {
                        id: mouseArea_about
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: {
                            drawer.close()
                            openFile("AboutPage.qml");
                        }
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 50
                        border.width: button_about.activeFocus ? 2 : 0
                        border.color: button_about.down ? "#17a81a" : "#21be2b"
                        color: mouseArea_about.containsMouse ? Theme.secondaryColor : drawer.background.color
                        radius: 4
                    }
                }
                Button {
                    id: button_settings
                    Layout.fillWidth: true
                    text: Config.tr("Settings")
                    font.pointSize: large_FontPointSize 
                    MouseArea {
                        id: mouseArea_settings
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: {
                            drawer.close()
                            openFile("SettingsPage.qml");
                        }
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 50
                        border.width: button_settings.activeFocus ? 2 : 0
                        border.color: button_settings.down ? "#17a81a" : "#21be2b"
                        color: mouseArea_settings.containsMouse ? Theme.secondaryColor : drawer.background.color
                        radius: 4
                    }
                }
                Button {
                    id: button_refresh
                    Layout.fillWidth: true
                    text: Config.tr("Refresh")
                    font.pointSize: large_FontPointSize 
                    MouseArea {
                        id: mouseArea_refresh
                        anchors.fill: parent
                        hoverEnabled: true
                        onReleased: {
                            drawer.close()
                            if (!isUpdate) main.update();
                        }
                    }
                    background: Rectangle {
                        implicitWidth: 200
                        implicitHeight: 50
                        border.width: button_refresh.activeFocus ? 2 : 0
                        border.color: button_refresh.down ? "#17a81a" : "#21be2b"
                        color: mouseArea_refresh.containsMouse ? Theme.secondaryColor : drawer.background.color
                        radius: 4
                    }
                }
            }
        }

        /*
        PullDownMenu {
            MenuItem {
                 id: item4menu
                 text: Config.tr("About")
                 onClicked: {
                      openFile("AboutPage.qml");
                 }
            }
            MenuItem {
                id: item1menu
                text: Config.tr("Settings")
                onClicked: {
                     openFile("SettingsPage.qml");
                }
            }
            MenuItem {
                id: item2menu
                text: Config.tr("Refresh")
                onClicked: {
                    if (!isUpdate) main.update();
                }
            }
         }
        */
         Column {
            anchors.fill: parent
            visible: Config.stationname == "Unknown" ? true : false
           // color: Config.transparency ? "transparent" : "black"
            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                color: Config.transparency ? "transparent" : "black"
                height: column_rect_height 
            }
            Text {
                id: empty_text
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: column_rect_height 
                color: Theme.primaryColor
                font.pointSize: large_FontPointSize 
                text: Config.tr("MeeCast")
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            Rectangle {
                anchors.top: empty_text.bottom
                width: parent.width
                height: top_rect_height 
                color: Config.transparency ? "transparent" : "black"
            }

            Loader {
                id: empty_background1
                visible: Config.transparency ? false : true
                anchors.top: empty_text.bottom
                anchors.left: parent.left
                width: parent.width
                height: top_rect_height
                sourceComponent: Image {source: Config.imagespath + "/mask_background.png"}
            }
            Rectangle {
                anchors.top: empty_background1.bottom
                width: parent.width
                height: 600*ratio 
                color: Config.transparency ? "transparent" : "black"
            }
            Label {
                horizontalAlignment: Text.AlignHCenter
                text: Config.tr("No locations are set up yet.")
                font.pixelSize: huge_PixelSize 
                color: Theme.secondaryColor
                wrapMode: Text.Wrap
                width: parent.width - 2*margin*ratio
                anchors.top: parent.top
                anchors.topMargin: 250*ratio
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
                anchors.bottomMargin: 150*ratio
            }
        }
    }

    //SilicaListView {
    ListView {
        id: listview
        anchors.fill: parent
        height: screen_height
        width: screen_width
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
            flipmoving = false
            //console.log("onFlickEnded")
            real_current_id = indexAt(contentX,contentY);
            Config._current_station_id(main.real_current_id)
            Config.saveConfig();
           // Config.refreshconfig();
        }
        onFlickStarted: {
            flipmoving = true
        }
        delegate: WeatherStationDelegate { id: weatherStationDelegate }
     }
}

