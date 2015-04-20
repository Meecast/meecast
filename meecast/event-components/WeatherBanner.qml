import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Weather 1.0

BackgroundItem {

    id: weatherData
    property bool autoRefresh: false
    property bool active: true 
    property QtObject meecastData

    Component.onCompleted: {
        meecastData = Qt.createQmlObject('import QtQuick 2.0; import org.meecast.data 1.0 as Meecast; Meecast.Data {}', weatherData)
    }

    function reload() {
        console.log("reload")
    }
    function save() {
    }

    visible: enabled
    enabled: enabled 
    height: enabled ? temperature.height + 2*(Theme.paddingLarge) : 0
    onClicked: { console.log ("Run MeeCast")
                 weatherData.meecastData.startMeeCast() }
    
    Image {
        id: icon
        anchors {
            top: parent.top
            bottom: parent.bottom
            topMargin: Theme.paddingMedium
            bottomMargin: Theme.paddingMedium
            left: parent.left
            leftMargin: isPortrait ? 0 : Theme.paddingMedium + Theme.paddingSmall
        }
        width: height
        source: weatherData.meecastData.forecastdata["item1_icon"]
    }
    Label {
        id: stationname
        text: weatherData.meecastData.nameString ? weatherData.meecastData.nameString : "MeeCast"
        color: Theme.primaryColor 
        font {
            pixelSize: Theme.fontSizeExtraLarge 
            family: Theme.fontFamilyHeading
        }
        truncationMode: TruncationMode.Fade
        anchors {
            left: icon.right
            leftMargin: isPortrait ? Theme.paddingSmall : Theme.paddingLarge
            top: parent.top
            right: temperature.left
        }
    }

    Label {
        id: description 
        text: weatherData.meecastData.forecastdata["item1_description"] ? weatherData.meecastData.forecastdata["item1_description"] : "MeeCast"
        font {
            pixelSize: isPortrait ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall
            family: Theme.fontFamilyHeading
        }
        color: Theme.secondaryColor
        truncationMode: TruncationMode.Fade
        anchors {
            left: icon.right
            top: stationname.bottom 
            topMargin: -(Theme.paddingSmall)
            leftMargin: isPortrait ? 2*Theme.paddingSmall : 2*Theme.paddingLarge
            right: temperature.left
        }
    }

    Label {
        id: temperature
        text: {
            if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
                return weatherData.meecastData.forecastdata["item1_temperature"] + '°' 
            }else{
                if (weatherData.meecastData.forecastdata["item1_temperature_low"] && weatherData.meecastData.forecastdata["item1_temperature_high"])
                    return weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + "/"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' 
                else
                    return ""
            }
        }
        y: isPortrait ? Theme.paddingLarge : Theme.paddingMedium
        anchors {
            right: parent.right
            rightMargin: Theme.paddingMedium
        }
        font {
            pixelSize: isPortrait ? Theme.fontSizeHuge : Theme.fontSizeExtraLarge
            family: Theme.fontFamilyHeading
        }


    }

    Connections {
        target: weatherData.meecastData 
        onRefreshWidget: {            
            console.log("Refresh MeeCast widget !!!!!!!!!!!!!!")
            stationname.text = weatherData.meecastData.nameString 
//            last_update.text = weatherData.meecastData.forecastdata["last_update"]
            if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
                temperature.text = weatherData.meecastData.forecastdata["item1_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
            }else{
                 temperature.text = weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + weatherData.meecastData.forecastdata["temperatureunit"] + "\n"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
            }
            icon.source = weatherData.meecastData.forecastdata["item1_icon"]
            description.text = weatherData.meecastData.forecastdata["item1_description"] ? weatherData.meecastData.forecastdata["item1_description"] : "MeeCast"
        }
    }

}
