import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: update 
    property int margin: 16
    property int screen_width : 854
    property bool event_widget_status: Config.eventwidget
    Rectangle {
        anchors.fill: parent
        color: Config.transparency ? "transparent" : "black"
    }

    PageHeader {
        title: Config.tr("Update")
    }

    Rectangle {
    	id: dataview
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
            height: 610
            color: "transparent"
        }

        Loader {
            id: empty_background
            anchors.top: top_rect.bottom
            anchors.left: parent.left
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: empty_background.bottom
            width: parent.width
            height: dataview.height - 610
            color: "transparent"
        }

        SilicaFlickable {
            anchors.fill: parent
            anchors.topMargin: 80
            contentHeight: 950 

            Column{
                anchors.fill: parent
                ComboBox {
                    label: Config.tr("Update interval")
                    currentIndex: -1
                    width: update.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("Never")
                            onClicked: {Config.update_interval(2147483647); } 
                        }
                        MenuItem {
                            text: Config.tr("15 minutes")
                            onClicked: {Config.update_interval(900); }
                        }
                        MenuItem {
                            text: Config.tr("30 minutes")
                            onClicked: { Config.update_interval(1800); }
                        }
                        MenuItem {
                            text: Config.tr("1 hour")
                            onClicked: { Config.update_interval(3600); }
                        }
                        MenuItem {
                            text: Config.tr("2 hours")
                            onClicked: { Config.update_interval(7200); }
                        }
                        MenuItem {
                            text: Config.tr("4 hours")
                            onClicked: { Config.update_interval(14400); }
                        }/*
                        MenuItem {
                            text: Config.tr("12 hours")
                            onClicked: { Config.update_interval(43200); }
                        }
                        */
                       /* 
                        MenuItem {
                            text: Config.tr("Dayily")
                            onClicked: { Config.update_interval(86400); }
                        }
                        */

                    }
                    Component.onCompleted: {
                        if (!(Config.updateinterval == 900 || Config.updateinterval == 1800 || Config.updateinterval == 3600 || Config.updateinterval == 7200 || Config.updateinterval == 14400 || Config.updateinterval == 43200 || Config.updateinterval == 86400))
                            currentIndex = 0
                        if (Config.updateinterval == 900)
                            currentIndex = 1
                        if (Config.updateinterval == 1800)
                            currentIndex = 2
                        if (Config.updateinterval == 3600)
                            currentIndex = 3
                        if (Config.updateinterval == 7200)
                            currentIndex = 4
                        if (Config.updateinterval == 14400)
                            currentIndex = 5
                        if (Config.updateinterval == 43200)
                            currentIndex = 6
                        if (Config.updateinterval == 86400)
                            currentIndex = 7
                    }
                }
            }
        }
    }
}
    

