import QtQuick 2.0
import Sailfish.Silica 1.0


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
            current_rect.color = getColor(cover.current_model("temp_high"));
        }else{
           temp_text.text = cover.current_model("temp") + '°'
           //current_rect.color = getColor(cover.current_model("temp"));
        }

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
        }
    }

    Label {
        id: stationname 
        anchors.top: parent.top
        anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        text: Config.stationname
        font.pixelSize: text.length > 20 ? 24 : 35 
    }
    Text {
        anchors.top: stationname.bottom
        id: temp_text
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
        source: Config.iconspath + "/" + Config.iconset + "/" + cover.current_model("pict") 
        width:  128
        height: 128
        anchors.top: stationname.bottom
        anchors.topMargin: 20
        smooth: true
    }
    Text {
        anchors.top: icon.bottom
        id: description
        width: parent.width 
        height: 130 
        color: "white"
        wrapMode:  TextEdit.WordWrap
        text: current_model("description")
        font.pointSize: 25 
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
        anchors.verticalCenter: parent.verticalCenter
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
