import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: units
    property int margin: 16

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    PageHeader {
        title: Config.tr("Measurement units")
    }


    Rectangle{
        anchors.fill: parent
        color: "transparent"
        Rectangle {
            id: top_rect
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 80 
            color: "transparent"
        }
        Rectangle {
            anchors.top: top_rect.bottom
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: "#999999"
        }
        Loader {
            id: background
            anchors.top: top_rect.bottom
            anchors.left: parent.left
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 274
            color: "black"
        }
        SilicaFlickable {
            anchors.fill: parent
            anchors.topMargin: 80
            contentHeight: 850 

            Column {
                anchors.fill: parent
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
                            text: Config.tr("mi/h")
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
                            if  (Config.windspeed_list()[i] == Config.windspeedunit)
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
                            if  (Config.pressure_list()[i] == Config.pressureunit)
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
                            if  (Config.visible_list()[i] == Config.visibleunit)
                                currentIndex = i 
                        }
                    }
                }
            }
        }
    }
}
