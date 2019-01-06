import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.meecast.meecastcover 1.0


CoverBackground {
    id: coverPage
    property bool active: status == Cover.Active || applicationActive;
    property bool activating: status == Cover.Activating;
    property bool deactivating: status == Cover.Deactivating;
    property bool isUpdate: false
    anchors.fill: parent
    property double angle: 0.0
    property int screen_height : coverPage.height
    property int screen_width : coverPage.width 
    property double ratio: coverPage.height/374

//  allowResize: true

    function current_model(name){
        return Current.getdata(0, name);
    }
    function current_temperature(){
        if (coverPage.current_model("temp") == undefined){
            temp_text.text = ""
            return;
        }
        if (coverPage.current_model("temp") == "N/A"){
            temp_text.text = ""
            if (coverPage.current_model("temp_high") != "N/A")
               temp_text.text =  coverPage.current_model("temp_high") + '°'
            if ((coverPage.current_model("temp_low")  != "N/A") && (coverPage.current_model("temp_high") != "N/A"))
               temp_text.text =  temp_text.text + "\n"
            if (coverPage.current_model("temp_low") != "N/A")
               temp_text.text = temp_text.text + coverPage.current_model("temp_low") + '°'
            if (coverPage.current_model("temp_high") != undefined && coverPage.current_model("temp_high") != "N/A") 
                background_rect.color = getColor(coverPage.current_model("temp_high"));
            else
                if (coverPage.current_model("temp_low") != undefined && coverPage.current_model("temp_low") != "N/A") 
                    background_rect.color = getColor(coverPage.current_model("temp_low"));
        }else{
           temp_text.text = coverPage.current_model("temp") + '°'
           background_rect.color = getColor(coverPage.current_model("temp"));
        }
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

    function update_data_on_page(){
        stationname.text = Config.stationname

        stationname.font.pixelSize = 35*ratio
        if (stationname.text.length > 15)
        	stationname.font.pixelSize = 30*ratio 
        if (stationname.text.length > 18)
        	stationname.font.pixelSize = 28*ratio
        if (stationname.text.length > 20)
        	stationname.font.pixelSize = 24*ratio 
        current_temperature()
        if (coverPage.current_model("current") != undefined){
            now.visible = !coverPage.current_model("current")
            if (coverPage.current_model("current") == true)
                now.text = Config.tr("Now")
            else
                now.text = Config.tr("Today")
            temp_text.font.bold = coverPage.current_model("current")
        }else{
            now.visible = false
            temp_text.font.bold = false
        }
        if (temp_text.text.length < 5)
            temp_text.font.pointSize = 44*ratio 
        else
            temp_text.font.pointSize = 40*ratio
        if (coverPage.current_model("pict") == undefined){
            icon.source = Config.iconspath + "/" + Config.iconset + "/49.png"
        }else{
            icon.source = Config.iconspath + "/" + Config.iconset + "/" + coverPage.current_model("pict")
        }
        if (coverPage.current_model("description") != undefined)
            description.text = coverPage.current_model("description")
        else
            description.text = ""
        source_image.source = Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
        if (Config.stationname == "Unknown") {
            description.text = Config.tr("No locations are set up yet.")
            stationname.text = "MeeCast"
            update_next_cover.enabled = false
            update_cover.enabled = false
            add_cover.enabled = true
        }else {
            if (Config.prevstationname == "" && Config.nextstationname == ""){
                add_cover.enabled = false 
                update_cover.enabled = true
                update_next_cover.enabled = false
            }else{
                add_cover.enabled = false 
                update_cover.enabled = false
                update_next_cover.enabled = true
            }
            if (Current.rowCount() == 0) {
                description.text = Config.tr("Looks like there's no info for this location.")
                }else{
                description.text = coverPage.current_model("description")
            }
        }
        if (description.text.length < 31)
            if (description.text.length < 10)
                description.font.pointSize = 32*ratio
            else 
                if (description.text.length < 20)
                    description.font.pointSize = 25*ratio
                else
                    description.font.pointSize = 20*ratio
        else
            description.font.pointSize = 16*ratio
        if ((Config.lastupdatecoverpage && coverPage.current_model("lastupdatetime") != undefined)){
            lastupdate.visible = true
        }else{
            lastupdate.visible = false
        }
        if (coverPage.current_model("lastupdatetime") != undefined){
            if ((coverPage.current_model("lastupdatetime").length + Config.tr("Last update:").length) < 25)
                lastupdate.text = Config.tr("Last update:") + " " + coverPage.current_model("lastupdatetime")
            else
                lastupdate.text = coverPage.current_model("lastupdatetime")
        }
    
        if (Config.logocoverpage) {
            source_image.visible = true
            icon.anchors.top = source_image.bottom
        } else {
            source_image.visible = false
            icon.anchors.top = stationname.bottom
        }

        if (Config.windcoverpage){
            icon.width = 0.374*screen_height 
            icon.height = 0.374*screen_height 
            if (coverPage.current_model("wind_speed") != undefined && coverPage.current_model("wind_speed") != "N/A"){
                wind_speed_text.text = (Config.windspeedunit == "Beaufort scale") ? coverPage.current_model("wind_speed") : coverPage.current_model("wind_speed") + ' ' + Config.tr(Config.windspeedunit)
                wind_speed_text.visible = true
                //description.anchors.top = wind_speed_text.bottom
            }else{
                wind_speed_text.visible = false
                //description.anchors.top = icon.bottom
            }
            if (coverPage.current_model("wind_direction") != undefined && coverPage.current_model("wind_direction") != "N/A"){
                wind_direction_background.visible = true
                wind_direction.visible = true
                //description.anchors.top = wind_direction_background.bottom
                if (coverPage.current_model("wind_direction") == undefined ||
                    coverPage.current_model("wind_direction") == "CALM" ||
                    coverPage.current_model("wind_direction") == "VAR" ||
                    coverPage.current_model("wind_direction") == "N/A"){
                    coverPage.angle = 0;
                }else{
                    coverPage.angle = coverPage.getAngle(coverPage.current_model("wind_direction"))
                }
            }else{
                wind_direction_background.visible = false
                wind_direction.visible = false
            }
        }else{
            icon.width = 0.342*screen_height
            icon.height = 0.342*screen_height
            wind_speed_text.visible = false
            wind_direction_background.visible = false
            wind_direction.visible = false
            //description.anchors.top = icon.bottom
        }
        isUpdate = false;

    }
    MeeCastCover {
        status: coverPage.activating
        onStatusChanged: { 
            console.log(" Change status ", coverPage.activating);		
            if (status){
                if (!Config.updating){
                    Config.refreshconfig3()
                    Config.check_and_update_station();
                    update_data_on_page();
                }else{
                    update_cover.enabled = false
                    update_next_cover.enabled = false
                    add_cover.enabled = false 
                    wind_speed_text.visible = false
                    wind_direction_background.visible = false
                    wind_direction.visible = false
                    source_image.visible = false
                    now.visible = false
                    lastupdate.visible = false
                    isUpdate = true;
                }
                if (!isUpdate && Config.stationname != "Unknown"){
                    add_cover.enabled = false;
                    update_cover.enabled = false;
                    update_next_cover.enabled = true;
                }
                if (!isUpdate && Config.stationname == "Unknown"){
                    update_next_cover.enabled = false;
                    update_cover.enabled = false;
                    add_cover.enabled = true;
                }
                if (!isUpdate && Config.stationname != "Unknown" && Config.nextstationname == "" && Config.prevstationname == "" ){
                    update_next_cover.enabled = false;
                    add_cover.enabled = false;
                    update_cover.enabled = true;
                }
                if (isUpdate){
                    update_next_cover.enabled = false;
                    add_cover.enabled = false;
                    update_cover.enabled = false;
                }
            }
        }
    }
    MeeCastCover {
        status: coverPage.deactivating
        onStatusChanged: { 
            console.log(" Change deactivating status ", coverPage.deactivating);		
            if (status){
                Config.configChangeStationOnMainPage();
            }
        }
    }

    Connections {
        target: Config
        onConfigReloadCoverPage: {
            console.log("onConfigReloadCoverPage");
            console.log("Config.filename", Config.data_filename);
            Current.reload_data(Config.data_filename);
            Current.update_model(0);
            update_data_on_page();
        }

        onConfigChanged: {
    //        Current.reload_data(Config.filename);
    //        Current.update_model(0);

//            console.log("CoverPage.qml onConfigChanged ", Config.stationname)
            Config.refreshconfig3()
  //          console.log("CoverPage.qml onConfigChanged ", Config.stationname)
            update_data_on_page();
        }
    }
    Rectangle {
        id: background_rect
        anchors.fill: parent
        opacity: 0.0
        color: "white"
    }
    Image {
        id: background_image
        source: Config.imagespath + "/cover.png"
        opacity: 0.1
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        height: sourceSize.height * width / sourceSize.width
    }
    Text {
        id: stationname
        visible: isUpdate ? false : true
        anchors.top: parent.top
        color: Theme.primaryColor
        anchors.horizontalCenter: parent.horizontalCenter
        text: Config.stationname == "Unknown" ? "MeeCast" : Config.stationname
        font.pixelSize: text.length > 20 ? 24 : (text.length > 18 ? 28*ratio : (text.length > 15 ? 30*ratio : 35*ratio))
    }
    Text {
        id: now
        width: parent.width/2  
        height: 30
        anchors.top: stationname.bottom 
        anchors.topMargin: -9 
        anchors.right: parent.right
        color: Theme.primaryColor
        visible: coverPage.current_model("current") != undefined && !coverPage.current_model("current")
        text: coverPage.current_model("current") == true ? Config.tr("Now") : Config.tr("Today")
        font.pointSize: 10*ratio
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: temp_text
        visible: Config.stationname == "Unknown" || isUpdate  ? false : true
        anchors.top: stationname.bottom
        anchors.right: parent.right 
        anchors.topMargin: -10 
        anchors.rightMargin: 0 
        anchors.leftMargin: 0 
        anchors.left: icon.right
        wrapMode: TextEdit.WordWrap
        height: 0.289*screen_height 
        color: Theme.primaryColor
        lineHeight: 0.7
        text: Current.temp + '°'
        font.pointSize: 40*ratio 
        font.bold: coverPage.current_model("current") != undefined && coverPage.current_model("current") ? true : false
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        Component.onCompleted: { current_temperature()}
    }
    Image {
        id: icon
        visible: isUpdate ? false : true
        source: (Config.stationname == "Unknown" || Current.rowCount() == 0 || coverPage.current_model("pict") == undefined) ? Config.iconspath + "/" + Config.iconset + "/49.png" : Config.iconspath + "/" + Config.iconset + "/" + coverPage.current_model("pict") 
        width:  Config.windcoverpage ? 0.342*screen_height : 0.374*screen_height
        height: Config.windcoverpage ? 0.342*screen_height : 0.374*screen_height
        anchors.top: Config.logocoverpage ? source_image.bottom : stationname.bottom
        anchors.topMargin: 5 
        smooth: true
    }
    Text {
        id: wind_speed_text
        text: (Config.windspeedunit == "Beaufort scale") ? coverPage.current_model("wind_speed") : coverPage.current_model("wind_speed") + ' ' + Config.tr(Config.windspeedunit)
        visible: Config.windcoverpage && !isUpdate && coverPage.current_model("wind_speed") != undefined && coverPage.current_model("wind_speed") != "N/A"
        anchors.top: temp_text.bottom
        anchors.right: parent.right
        anchors.rightMargin: Theme.paddingLarge
        height: 30
        color: Theme.primaryColor
        font.pointSize: 14*ratio
        verticalAlignment: Text.AlignVCenter
    }
    Image {
        id: wind_direction_background
        visible: Config.windcoverpage && !isUpdate && coverPage.current_model("wind_direction") != "N/A"
        source: Config.imagespath + "/wind_direction_background.png"
        anchors.top: temp_text.bottom
        anchors.right: wind_speed_text.left
        anchors.rightMargin: 4
        width: 30
        height: 30
        smooth: true
    }
    Image {
        id: wind_direction
        visible: Config.windcoverpage && !isUpdate && coverPage.current_model("wind_direction") != "N/A"
        source: Config.imagespath + "/wind_direction_arrow.png"
        anchors.top: temp_text.bottom
        anchors.right: wind_speed_text.left
        anchors.rightMargin: 4
        width: 30
        height: 30
        smooth: true
        transform: Rotation {
            id : transform
            origin.x: 15
            origin.y: 15
            angle: coverPage.angle
        }
    }
    Text {
        id: description
        visible: isUpdate ? false : true
//        anchors.top: Config.windcoverpage && coverPage.current_model("wind_speed") != "N/A" ? wind_direction.bottom : icon.bottom
        anchors.top: icon.bottom
        anchors.topMargin: -10
        width: parent.width
        // height: 0.36*screen_height
        elide : Text.ElideRight
        color: Theme.primaryColor
        wrapMode:  TextEdit.WordWrap
        text: Config.stationname == "Unknown" ? Config.tr("No locations are set up yet.") : (Current.rowCount() == 0) ? "Looks like there's no info for this location." : coverPage.current_model("description")
        font.pointSize: text.length < 20 ? 25*ratio : 16*ratio
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        id: lastupdate 
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        visible: (Config.lastupdatecoverpage && !isUpdate && coverPage.current_model("lastupdatetime") != undefined) ? true : false 
        anchors.horizontalCenter: parent.horizontalCenter
        text: coverPage.current_model("lastupdatetime") == undefined ? "MeeCast" : ((coverPage.current_model("lastupdatetime").length + Config.tr("Last update:").length) < 25 ?  Config.tr("Last update:") + " " + coverPage.current_model("lastupdatetime") : coverPage.current_model("lastupdatetime"))
        font.pixelSize: 20*ratio 
        color: Theme.primaryColor
    }

    Image {
        visible: (Config.logocoverpage && !isUpdate)  ? true : false
        id: source_image 
        source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
        anchors.top: stationname.bottom
        width: 0.214*screen_height 
        smooth: true    
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: icon.horizontalCenter
      //  scale: 0.8
    }
    Text {
        id: title
        visible: isUpdate ? true : false
        anchors.top: parent.top
        color: Theme.primaryColor
        anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        text: "MeeCast" 
        font.pixelSize: 35 
    }

    Image {
        id: refresh_image
        source: "image://theme/icon-cover-refresh"
        anchors.top: parent.top 
        anchors.topMargin: 80 
        visible: isUpdate ? true : false
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        height: 100
        RotationAnimation on rotation {
            duration: 2000; 
            loops: Animation.Infinite; 
            running : isUpdate ? active : false
            from: 0; to: 360
        }
    }

    Text {
        text: Config.tr("Updating forecast")
        horizontalAlignment: Text.AlignHLeft; 
        anchors.top: parent.top 
        anchors.topMargin: 190 
        color: Theme.primaryColor
        //anchors.centerIn: parent
        visible: isUpdate ? true : false
        font.pointSize: 20; 
        NumberAnimation on x { 
            id: text_anim; 
            from: 250; to: -300; 
            duration: 7000; 
            loops: Animation.Infinite; 
            running : isUpdate ? active : false
        }  
    }

    CoverActionList {
        id: update_next_cover 
        //enabled: (Config.stationname == "Unknown") || isUpdate || (Config.nextstationname == "" && Config.prevstationname == "") ? false : true
        enabled: true
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: { Config.changestation();}
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                update_cover.enabled = false
                update_next_cover.enabled = false
                add_cover.enabled = false 
                isUpdate = true;  
                wind_speed_text.visible = false
                wind_direction_background.visible = false
                wind_direction.visible = false
                source_image.visible = false
                now.visible = false
                lastupdate.visible = false
                Config.updatestations(); 
            }
        }
    }
    CoverActionList {
        id: add_cover 
        //enabled: (isUpdate || Config.stationname != "Unknown") ? false : true
        enabled: false
        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: { 
                app.activate()
                pageStack.push(Qt.resolvedUrl("../pages/SourcePage.qml"))
	        }
        }
    }
    CoverActionList {
        id: update_cover 
        //enabled: (isUpdate || Config.stationname == "Unknown" || Config.nextstationname != "" || Config.prevstationname != "" ) ? false : true
        enabled: false 
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
	        onTriggered: { 
                update_cover.enabled = false
                update_next_cover.enabled = false
                add_cover.enabled = false 
                Config.updatestations(); 
                isUpdate = true;  
                wind_speed_text.visible = false
                wind_direction_background.visible = false
                wind_direction.visible = false
                source_image.visible = false
                now.visible = false
                lastupdate.visible = false
	        }
        }
    }

    Component.onDestruction : {   
        console.log("Component.onDestruction")
    }
}
