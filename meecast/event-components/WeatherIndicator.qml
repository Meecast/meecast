import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Weather 1.0

Row {
    id: root
    property bool autoRefresh: false
    property bool active: true 
    property QtObject meecastData
    property alias temperatureFont: temperatureLabel.font

    anchors.horizontalCenter: parent.horizontalCenter
    height: icon.height
    spacing: Theme.paddingMedium
    visible: !!weather
    enabled: enabled 


    Component.onCompleted: {
        meecastData = Qt.createQmlObject('import QtQuick 2.0; import org.meecast.data 1.0 as Meecast; Meecast.Data {}', root)
    }
    function refresh(){

        icon.source = root.meecastData.forecastdata["item1_icon"]
        if (root.meecastData.forecastdata["item1_temperature"] && root.meecastData.forecastdata["item1_temperature"] != "N/A"){
            temperatureLabel.text = root.meecastData.forecastdata["item1_temperature"] + '°' + root.meecastData.forecastdata["temperatureunit"]
        }else{
             temperatureLabel.text = root.meecastData.forecastdata["item1_temperature_low"] +  '°' + root.meecastData.forecastdata["temperatureunit"] + "/"+  root.meecastData.forecastdata["item1_temperature_high"] + '°' + root.meecastData.forecastdata["temperatureunit"]
        }
    }

    Image {
        id: icon
        anchors.verticalCenter: parent.verticalCenter
        source: root.meecastData.forecastdata["item1_icon"] 
        sourceSize.width: Theme.iconSizeSmallPlus
        sourceSize.height: Theme.iconSizeSmallPlus
    }

    Label {
        id: temperatureLabel 
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
    Connections {
        target: root.meecastData 
        onRefreshWidget: {            
            console.log("Refresh MeeCast widget in Indicator 1!!!!!!!!!!!!!!")
            root.refresh();
        }
    }

}
