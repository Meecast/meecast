import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.meecast.meecastcover 1.0


CoverBackground {
    id: coverPage
    property bool active: status == coverPage.Active || applicationActive;
    property bool isUpdate: false
    anchors.fill: parent

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
            //current_rect.color = getColor(coverPage.current_model("temp_high"));
        }else{
           temp_text.text = coverPage.current_model("temp") + '°'
           //current_rect.color = getColor(coverPage.current_model("temp"));
        }

    }

    MeeCastCover {
        status: coverPage.active
    }
    Connections {
        target: Config
        onConfigChanged: {
            stationname.text = Config.stationname
            current_temperature()
            icon.source = Config.iconspath + "/" + Config.iconset + "/" + coverPage.current_model("pict")
            if (coverPage.current_model("description") != undefined)
                description.text = coverPage.current_model("description")
            else
                description.text = ""
            source_image.source = Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
            if (Config.stationname == "Unknown") {
                description.text = Config.tr("No locations are set up yet.")
            }else {
            if (Current.rowCount() == 0) {
                    description.text = Config.tr("Looks like there's no info for this location.")}
                else{
                    description.text = current_model("description")
                }
            }
            if (description.text.length < 31)
                if (description.text.length < 10)
                    description.font.pointSize = 32
                else
                    description.font.pointSize = 20
            else
                description.font.pointSize = 16

            isUpdate = false;
        }
    }

    Label {
        id: stationname
        visible: isUpdate ? false : true
        anchors.top: parent.top
//        anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        text: Config.stationname == "Unknown" ? "MeeCast" : Config.stationname
        font.pixelSize: text.length > 20 ? 24 : 35 
    }
    Text {
        id: temp_text
        visible: Config.stationname == "Unknown" || isUpdate  ? false : true
        anchors.top: stationname.bottom
        anchors.right: parent.right 
        anchors.topMargin: 15 
        anchors.rightMargin: 0 
        anchors.leftMargin: 0 
        anchors.bottomMargin: 15 
        anchors.left: icon.right
        wrapMode:  TextEdit.WordWrap
        height: 108 
        color: "white"
        text: Current.temp + '°'
        font.pointSize: 42 
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
        Component.onCompleted: { current_temperature()}
    }
    Image {
        id: icon
        visible: isUpdate ? false : true
        source: (Config.stationname == "Unknown" || Current.rowCount() == 0) ? Config.iconspath + "/" + Config.iconset + "/49.png" : Config.iconspath + "/" + Config.iconset + "/" + coverPage.current_model("pict") 
        width:  128
        height: 128
        anchors.top: stationname.bottom
        anchors.topMargin: 20
        smooth: true
    }
    Text {
        id: description
        visible: isUpdate ? false : true
        anchors.top: icon.bottom
        width: parent.width 
        height: 80 
        color: "white"
        wrapMode:  TextEdit.WordWrap
        text: Config.stationname == "Unknown" ? Config.tr("No locations are set up yet.") : (Current.rowCount() == 0) ? "Looks like there's no info for this location." : current_model("description")
        font.pointSize: text.length < 20 ? 25 : 16
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    Image {
        visible: isUpdate ? false : true
        id: source_image 
        source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
        anchors.bottom: parent.bottom
        height: 40 
    //    anchors.top: description.bottom
        smooth: true    
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
     //   anchors.verticalCenter: parent.verticalCenter
        scale: 0.8
    }
    Label {
        id: title
        visible: isUpdate ? true : false
        anchors.top: parent.top
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
        id: ordinary
        enabled: isUpdate ? false : true
        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: { Config.changestation();}
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: {
                Config.updatestations(); 
                isUpdate = true;  
            }
        }
    }
}
