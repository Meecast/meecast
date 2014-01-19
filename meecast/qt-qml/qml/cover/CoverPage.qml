import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.meecast.meecastcover 1.0


CoverBackground {
    id: cover
    function current_model(name){
        return Current.getdata(0, name);
    }
    function current_temperature(){
        if (cover.current_model("temp") == undefined){
            temp_text.text = ""
            return;
        }
        if (cover.current_model("temp") == "N/A"){
            temp_text.text = ""
            if (cover.current_model("temp_high") != "N/A")
               temp_text.text =  cover.current_model("temp_high") + '°'
            if ((cover.current_model("temp_low")  != "N/A") && (cover.current_model("temp_high") != "N/A"))
               temp_text.text =  temp_text.text + "\n"
            if (cover.current_model("temp_low") != "N/A")
               temp_text.text = temp_text.text + cover.current_model("temp_low") + '°'
            //current_rect.color = getColor(cover.current_model("temp_high"));
        }else{
           temp_text.text = cover.current_model("temp") + '°'
           //current_rect.color = getColor(cover.current_model("temp"));
        }

    }
    property bool active: status == Cover.Active || applicationActive;

    MeeCastCover {
        status: cover.active
    }
    Connections {
        target: Config
        onConfigChanged: {
            stationname.text = Config.stationname
            current_temperature()
            icon.source = Config.iconspath + "/" + Config.iconset + "/" + cover.current_model("pict")
            if (cover.current_model("description") != undefined)
                description.text = cover.current_model("description")
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
                if (description.text.length < 12)
                    description.font.pointSize = 32
                else
                    description.font.pointSize = 25
            else
                description.font.pointSize = 18
        }
    }

    Label {
        id: stationname
        anchors.top: parent.top
        anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        text: Config.stationname == "Unknown" ? "MeeCast" : Config.stationname
        font.pixelSize: text.length > 20 ? 24 : 35 
    }
    Text {
        id: temp_text
        visible: Config.stationname == "Unknown" ? false : true
        anchors.top: stationname.bottom
        anchors.right: parent.right 
        anchors.rightMargin: 20 
        anchors.topMargin: 20 
        anchors.left: icon.right
        wrapMode:  TextEdit.WordWrap
        height: 128 
        color: "white"
        text: Current.temp + '°'
        font.pointSize: 53 
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
//        anchors.horizontalCenter: parent.horizontalCenter
//        anchors.verticalCenter: parent.verticalCenter
        Component.onCompleted: { current_temperature()}
    }
    Image {
        id: icon
        source: (Config.stationname == "Unknown" || Current.rowCount() == 0) ? Config.iconspath + "/" + Config.iconset + "/49.png" : Config.iconspath + "/" + Config.iconset + "/" + cover.current_model("pict") 
        width:  128
        height: 128
        anchors.top: stationname.bottom
        anchors.topMargin: 20
        smooth: true
    }
    Text {
        id: description
        anchors.top: icon.bottom
        width: parent.width 
        height: 120 
        color: "white"
        wrapMode:  TextEdit.WordWrap
        text: Config.stationname == "Unknown" ? Config.tr("No locations are set up yet.") : (Current.rowCount() == 0) ? "Looks like there's no info for this location." : current_model("description")
        font.pointSize: text.length < 40 ? 25 : 18
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    Image {
        id: source_image 
        source: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"
        anchors.bottom: parent.bottom
        anchors.top: description.bottom
        smooth: true    
        fillMode: Image.PreserveAspectFit
        anchors.horizontalCenter: parent.horizontalCenter
     //   anchors.verticalCenter: parent.verticalCenter
        scale: 0.8
    }
    CoverActionList {
        CoverAction {
            onTriggered: { Config.changestationback(); }
        }
        CoverAction {
            onTriggered: { Config.changestation();}
        }
    }
}
