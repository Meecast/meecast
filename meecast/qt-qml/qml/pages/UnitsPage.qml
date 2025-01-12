import QtQuick 2.2
import Sailfish.Silica 1.0


Page {
    id: units
    property int margin: Theme.paddingSmall

    Rectangle {
        anchors.fill: parent
        color: {
            if (Config.transparency == "transparent"){
                return "transparent"
            }else{
                if (Theme.primaryColor == "#000000"){
                    return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                }else{
                    return "black"
                }
            }
        }
    }

    Rectangle{
        anchors.fill: parent
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent"

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            color: "transparent"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 610
            color: "transparent"
        }
    }

     
    SilicaFlickable {
            anchors.fill: parent
            anchors.topMargin: Theme.paddingLarge
            contentHeight: column.height
            contentWidth: parent.width
            VerticalScrollDecorator {}

            Column {
                anchors.fill: parent
                spacing: Theme.paddingLarge
                id: column
                PageHeader {
                    title: Config.tr("Measurement units")
                }
                ComboBox {
                    label: Config.tr("Temperature units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("Celsius")
                            onClicked: { Config.temperature_unit("C") } 
                        }
                        MenuItem {
                            text: Config.tr("Fahrenheit")
                            onClicked: { Config.temperature_unit("F") }
                        }
                    }
                    Component.onCompleted: {
                        for (var i=0; i<Config.temperature_list().length; i++) {
                            if  (Config.temperature_list()[i] == Config.temperatureunit)
                                currentIndex = i 
                        }
                    }
                }

                ComboBox {
                    label: Config.tr("Wind speed units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("m/s")
                            onClicked: { Config.windspeed_unit("0") } 
                        }
                        MenuItem {
                            text: Config.tr("km/h")
                            onClicked: { Config.windspeed_unit("1") } 
                        }
                        MenuItem {
                            text: Config.tr("mph")
                            onClicked: { Config.windspeed_unit("2") } 
                        }
                        MenuItem {
                            text: Config.tr("Knots")
                            onClicked: { Config.windspeed_unit("3") } 
                        }
                        MenuItem {
                            text: Config.tr("Beaufort scale")
                            onClicked: { Config.windspeed_unit("4") } 
                        }

                    }
                    Component.onCompleted: {
                        for (var i=0; i<Config.windspeed_list().length; i++) {
                            if  (Config.windspeed_list()[i] == Config.tr(Config.windspeedunit))
                                currentIndex = i 
                        }
                    }
                }
                ComboBox {
                    label: Config.tr("Pressure units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("mbar")
                            onClicked: { Config.pressure_unit("0") } 
                        }
                        MenuItem {
                            text: Config.tr("Pa")
                            onClicked: { Config.pressure_unit("1") } 
                        }
                        MenuItem {
                            text: Config.tr("mmHg")
                            onClicked: { Config.pressure_unit("2") } 
                        }
                    }
                    Component.onCompleted: {
                        for (var i=0; i<Config.pressure_list().length; i++) {
                            if  (Config.pressure_list()[i] == Config.tr(Config.pressureunit))
                                currentIndex = i 
                        }
                    }
                }
                ComboBox {
                    label: Config.tr("Visible units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("m")
                            onClicked: { Config.visible_unit("0") } 
                        }
                        MenuItem {
                            text: Config.tr("km")
                            onClicked: { Config.visible_unit("1") } 
                        }
                        MenuItem {
                            text: Config.tr("mi")
                            onClicked: { Config.visible_unit("2") } 
                        }
                    }
                    Component.onCompleted: {
                        for (var i=0; i<Config.visible_list().length; i++) {
                            if  (Config.visible_list()[i] == Config.tr(Config.visibleunit))
                                currentIndex = i 
                        }
                    }
                }
            }
        }
}
