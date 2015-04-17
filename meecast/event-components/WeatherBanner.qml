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
    height: enabled ? temperatureLabel.height + 2*(isPortrait ? Theme.paddingLarge : Theme.paddingMedium) : 0

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
        font {
            pixelSize: isPortrait ? Theme.fontSizeHuge : Theme.fontSizeExtraLarge
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
            pixelSize: isPortrait ? Theme.fontSizeSmall : Theme.fontSizeHuge
            family: Theme.fontFamilyHeading
        }
        truncationMode: TruncationMode.Fade
        anchors {
            left: icon.right
            leftMargin: isPortrait ? Theme.paddingSmall : Theme.paddingLarge
            bottom: parent.bottom
            right: temperature.left
        }
    }
    Label {
        id: temperature
        text: {
            if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
                return weatherData.meecastData.forecastdata["item1_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
            }else{
                if (weatherData.meecastData.forecastdata["item1_temperature_low"] && weatherData.meecastData.forecastdata["item1_temperature_high"])
                    return weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + weatherData.meecastData.forecastdata["temperatureunit"] + "\n"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
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

}
