import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Weather 1.0

BackgroundItem {

    id: weatherData
    property bool autoRefresh: false
    property bool active: true 
    property QtObject meecastData
    property bool expanded

    onClicked: expanded = !expanded

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
    height: enabled ? column.height : 0
    
    Column {
        id: column
        width: parent.width
        anchors {
            left: parent.left
        }
        Row {
            id: row
            spacing: Theme.paddingMedium
            anchors.horizontalCenter: parent.horizontalCenter
            height: Theme.itemSizeSmall
            anchors {
                left: parent.left
            }

            Image {
                id: icon
                height: parent.height 
                width: height
                source: weatherData.meecastData.forecastdata["item1_icon"]
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
                    top: parent.top
                }
                font {
                    pixelSize: isPortrait ? Theme.fontSizeHuge : Theme.fontSizeExtraLarge
                    family: Theme.fontFamilyHeading
                }
            }

            Column {
                Label {
                    id: stationname
                    text: weatherData.meecastData.nameString ? weatherData.meecastData.nameString : "MeeCast"
                    color: Theme.primaryColor 
                    horizontalAlignment: Text.AlignHCenter
                    font {
                        pixelSize: Theme.fontSizeLarge 
                        family: Theme.fontFamilyHeading
                    }
                    truncationMode: TruncationMode.Fade
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
                }
            }

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
