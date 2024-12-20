import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Weather 1.0

BackgroundItem {

    id: weatherData
    property bool autoRefresh: false
    property bool active: true
    property QtObject meecastData
    property bool expanded

    onClicked: {expanded = !expanded; weatherData.refresh()}
    onPressAndHold: { weatherData.meecastData.startMeeCast()}

    Component.onCompleted: {
        meecastData = Qt.createQmlObject('import QtQuick 2.0; import org.meecast.data 1.0 as Meecast; Meecast.Data {}', weatherData)
    }

    function refresh(){

        icon.source = weatherData.meecastData.forecastdata["item1_icon"]
        stationname.text = weatherData.meecastData.nameString
        description.text = weatherData.meecastData.forecastdata["item1_description"] ? weatherData.meecastData.forecastdata["item1_description"] : "MeeCast"

        if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
            temperature.text = weatherData.meecastData.forecastdata["item1_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
        }else{
             temperature.text = weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + weatherData.meecastData.forecastdata["temperatureunit"] + "/"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
        }

        if (expanded){

            //last_update.text = weatherData.meecastData.forecastdata["last_update_text"] + " " + weatherData.meecastData.forecastdata["last_update"]

            day_name_2.text = weatherData.meecastData.forecastdata["item2_short_day_name"]
            icon2.source = weatherData.meecastData.forecastdata["item2_icon"]

            //if ( weatherData.meecastData.forecastdata["item2_temperature_low"] == "N/A" ) {
            //    temperature_2_low.text = weatherData.meecastData.forecastdata["item2_temperature"] + '°'
            //} else {
            //    temperature_2_low.text = weatherData.meecastData.forecastdata["item2_temperature_low"] + '°'
            //}

            //if ( weatherData.meecastData.forecastdata["item2_temperature_high"] == "N/A" ) {
            //    temperature_2_high.text = weatherData.meecastData.forecastdata["item2_temperature"] + '°'
            //} else {
            //    temperature_2_high.text = weatherData.meecastData.forecastdata["item2_temperature_high"] + '°'
            //}


            day_name_2.text = weatherData.meecastData.forecastdata["item2_short_day_name"]
            icon2.source = weatherData.meecastData.forecastdata["item2_icon"]
            temperature_2_low.text = weatherData.meecastData.forecastdata["item2_temperature_low"] + '°'
            temperature_2_high.text = weatherData.meecastData.forecastdata["item2_temperature_high"] + '°'

            day_name_3.text = weatherData.meecastData.forecastdata["item3_short_day_name"]
            icon3.source = weatherData.meecastData.forecastdata["item3_icon"]
            temperature_3_low.text = weatherData.meecastData.forecastdata["item3_temperature_low"] + '°'
            temperature_3_high.text = weatherData.meecastData.forecastdata["item3_temperature_high"] + '°'

            day_name_4.text = weatherData.meecastData.forecastdata["item4_short_day_name"]
            icon4.source = weatherData.meecastData.forecastdata["item4_icon"]
            temperature_4_low.text = weatherData.meecastData.forecastdata["item4_temperature_low"] + '°'
            temperature_4_high.text = weatherData.meecastData.forecastdata["item4_temperature_high"] + '°'

            day_name_5.text = weatherData.meecastData.forecastdata["item5_short_day_name"]
            icon5.source = weatherData.meecastData.forecastdata["item5_icon"]
            temperature_5_low.text = weatherData.meecastData.forecastdata["item5_temperature_low"] + '°'
            temperature_5_high.text = weatherData.meecastData.forecastdata["item5_temperature_high"] + '°'

            day_name_6.text = weatherData.meecastData.forecastdata["item6_short_day_name"]
            icon6.source = weatherData.meecastData.forecastdata["item6_icon"]
            temperature_6_low.text = weatherData.meecastData.forecastdata["item6_temperature_low"] + '°'
            temperature_6_high.text = weatherData.meecastData.forecastdata["item6_temperature_high"] + '°'

            day_name_7.text = weatherData.meecastData.forecastdata["item7_short_day_name"]
            icon7.source = weatherData.meecastData.forecastdata["item7_icon"]
            temperature_7_low.text = weatherData.meecastData.forecastdata["item7_temperature_low"] + '°'
            temperature_7_high.text = weatherData.meecastData.forecastdata["item7_temperature_high"] + '°'

            last_update.text = weatherData.meecastData.forecastdata["last_update_text"] + " " + weatherData.meecastData.forecastdata["last_update"]
        }

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
            id: current_row

            anchors.horizontalCenter: parent.horizontalCenter
            height: Theme.itemSizeSmall

            Image {
                id: icon
                height: parent.height
                width: height
                anchors.verticalCenter: parent.verticalCenter
                source: weatherData.meecastData.forecastdata["item1_icon"]
            }

            Item {
                width: Theme.paddingMedium
                height: 1
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
                font.pixelSize: Theme.fontSizeExtraLarge
                anchors.verticalCenter: parent.verticalCenter
            }

            Item {
                width: Theme.paddingLarge
                height: 1
            }

            Column {
                id: location_desc
                Label {
                    id: stationname
                    text: weatherData.meecastData.nameString ? weatherData.meecastData.nameString : "MeeCast"
                    color: Theme.highlightColor
                    horizontalAlignment: Text.AlignHCenter
                    font {
                        pixelSize: Theme.fontSizeLarge
                        family: Theme.fontFamilyHeading
                    }
                    truncationMode: TruncationMode.Fade
                    width: Math.min(implicitWidth,
                       column.width - icon.width - 2*Theme.paddingMedium
                       - temperature.width - Theme.horizontalPageMargin)
                }

                Label {
                    id: description
                    text: weatherData.meecastData.forecastdata["item1_description"] ? weatherData.meecastData.forecastdata["item1_description"] : "MeeCast"
                    horizontalAlignment: Text.AlignHCenter
                    font {
                        pixelSize: isPortrait ? Theme.fontSizeExtraSmall : Theme.fontSizeSmall
                        family: Theme.fontFamilyHeading
                    }
                    color: lipstickSettings.lowPowerMode ? Theme.highlightColor : Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    width: Math.min(implicitWidth,
                       column.width - icon.width - 2*Theme.paddingMedium
                       - temperature.width - Theme.horizontalPageMargin)
                }
            }
        }

        Row {
            id: forecasts_row

            height: 0
            opacity: 0.0
            spacing: Theme.paddingLarge
            anchors.horizontalCenter: parent.horizontalCenter
            states: State {
                name: "expanded"
                when: weatherData.expanded
                PropertyChanges {
                    target: forecasts_row
                    opacity: 1.0
                    height: 2.4*(Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall)
                }
            }

            Row {
                id: forecast2_row
                Column {
                    id: icon_day_column2
                    Label {
                        id: day_name_2
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item2_short_day_name"]
                    }
                    Label {
                        id: temperature_2_high
                        anchors.bottom: icon_2.top
                        anchors.bottomMargin: -Theme.paddingSmall
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item2_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon2
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item2_icon"]
                    }
                    Label {
                        id: temperature_2_low
                        anchors.bottom: icon_2.top
                        anchors.bottomMargin: -Theme.paddingSmall
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item2_temperature_low"]  + '°'
                    }
                }
            }

            Row {
                id: forecast3_row
                Column {
                    id: icon_day_column3
                    Label {
                        id: day_name_3
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item3_short_day_name"]
                    }
                    Label {
                        id: temperature_3_high
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item3_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon3
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item3_icon"]
                    }
                    Label {
                        id: temperature_3_low
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item3_temperature_low"]  + '°'
                    }
                }
            }

            Row {
                id: forecast4_row
                Column {
                    id: icon_day_column4
                    Label {
                        id: day_name_4
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item4_short_day_name"]
                    }
                    Label {
                        id: temperature_4_high
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item4_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon4
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item4_icon"]
                    }
                    Label {
                        id: temperature_4_low
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item4_temperature_low"]  + '°'
                    }
                }
            }

            Row {
                id: forecast5_row
                Column {
                    id: icon_day_column5
                    Label {
                        id: day_name_5
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item5_short_day_name"]
                    }
                    Label {
                        id: temperature_5_high
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item5_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon5
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item5_icon"]
                    }
                    Label {
                        id: temperature_5_low
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item5_temperature_low"]  + '°'
                    }
                }
            }

            Row {
                id: forecast6_row
                Column {
                    id: icon_day_column6
                    Label {
                        id: day_name_6
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item6_short_day_name"]
                    }
                    Label {
                        id: temperature_6_high
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item6_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon6
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item6_icon"]
                    }
                    Label {
                        id: temperature_6_low
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item6_temperature_low"]  + '°'
                    }
                }
            }

            Row {
                id: forecast7_row
                Column {
                    id: icon_day_column7
                    Label {
                        id: day_name_7
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item7_short_day_name"]
                    }
                    Label {
                        id: temperature_7_high
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item7_temperature_high"]  + '°'
                    }
                    Image {
                        id: icon7
                        height: Screen.sizeCategory >= Screen.Large ? Theme.itemSizeLarge : Theme.itemSizeSmall
                        width: height
                        source: weatherData.meecastData.forecastdata["item7_icon"]
                    }
                    Label {
                        id: temperature_7_low
                        font.pixelSize: Theme.fontSizeSmall
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        text: weatherData.meecastData.forecastdata["item7_temperature_low"]  + '°'
                    }
                }
            }
        }

        Row {
            height: 0
            opacity: 0.0
            id: lastupdate_row
            width: parent.width
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin
            states: State {
                name: "expanded"
                when: weatherData.expanded
                PropertyChanges {
                    target: lastupdate_row
                    opacity: 1.0
                    height: Theme.itemSizeExtrasSmall
                }
            }

            Label {
                id: last_update
                width: parent.width
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                horizontalAlignment: Text.AlignRight
                text: "Last update: " + weatherData.meecastData.forecastdata["last_update"]
            }
        }
    }

    Connections {
        target: weatherData.meecastData
        onRefreshWidget: {
            console.log("Refresh MeeCast widget !!!!!!!!!!!!!!")
            weatherData.refresh();
        }
    }

}
