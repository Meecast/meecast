import QtQuick 2.0
import Sailfish.Silica 1.0
import "jsUtils.js" as Utils

Page {
    id: update 
    allowedOrientations: Utils.decideOrientation();
    property int margin: Theme.paddingSmall
    property int screen_width : 854
    property bool event_widget_status: Config.eventwidget
    Rectangle {
        anchors.fill: parent
        color: {
            if (Config.transparency){
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

        Column{
            anchors.fill: parent
            PageHeader {
                title: Config.tr("Update")
            }

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
    

