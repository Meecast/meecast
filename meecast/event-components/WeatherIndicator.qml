import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Weather 1.0

Row {
    id: root
    property bool autoRefresh: false
    property bool active: true 
    property QtObject meecastData

    anchors.horizontalCenter: parent.horizontalCenter
    height: icon.height
    spacing: Theme.paddingMedium
    visible: !!weather

    Image {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        source: root.meecastData.forecastdata["item1_icon"] 
        sourceSize.width: Theme.iconSizeSmallPlus
        sourceSize.height: Theme.iconSizeSmallPlus
    }

    Label {
        id: temperature
        anchors.verticalCenter: icon.verticalCenter
        text: {
            if (root.meecastData.forecastdata["item1_temperature"] && root.meecastData.forecastdata["item1_temperature"] != "N/A"){
                return root.meecastData.forecastdata["item1_temperature"] + '°' 
            }else{
                if (root.meecastData.forecastdata["item1_temperature_low"] && root.meecastData.forecastdata["item1_temperature_high"])
                    return root.meecastData.forecastdata["item1_temperature_low"] +  '°' + "/"+  root.meecastData.forecastdata["item1_temperature_high"] + '°' 
                else
                    return ""
            }
        }
        color: Theme.primaryColor
        font {
            pixelSize: Theme.fontSizeExtraLarge
            family: Theme.fontFamilyHeading
        }
    }

}
