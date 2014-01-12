import QtQuick 2.0
import Sailfish.Silica 1.0


CoverBackground {
    id: cover
    function current_model(name){
        return Current.getdata(0, name);
    }
    Connections {
        target: Config
        onConfigChanged: {
            console.log("!!!!!!!!!!!!!11end update station name = "+Config.stationname);
            stationname.text = Config.stationname
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
        width: 300 
        height: 84
        color: "white"
        text: Current.temp + '°'
        font.pointSize: 26
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        Component.onCompleted: {
            if (cover.current_model("temp") == "N/A"){
                temp_text.text = ""
                if (cover.current_model("temp_high") != "N/A")
                   temp_text.text =  cover.current_model("temp_high") + '°'
                if ((cover.current_model("temp_low")  != "N/A") && (cover.current_model("temp_high") != "N/A"))
                   temp_text.text =  temp_text.text + " / "
                if (model.temp_low != "N/A")
                   temp_text.text = temp_text.text + cover.current_model("temp_low") + '°'
                current_rect.color = getColor(cover.current_model("temp_high"));
            }else{
               temp_text.text = cover.current_model("temp") + '°'
               //current_rect.color = getColor(cover.current_model("temp"));
            }
        }
    }
}
