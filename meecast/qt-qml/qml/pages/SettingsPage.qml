import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: settings
    property int margin: 16
    function openFile(file)
    {
        pageStack.push(Qt.resolvedUrl(file))
    }
    ListModel {
        id: settingsModel
        ListElement {
            page: "StationsPage.qml"
            title: "Manage locations"
        }
        ListElement {
            page: "UnitsPage.qml"
            title: "Measurement units"
        }
	/* 
        ListElement {
            page: "SourcePage.qml"
            title: "Source"
        }*/
        ListElement {
            page: "VisualsPage.qml"
            title: "Appearance"
        }
        ListElement {
            page: "UpdatePage.qml"
            title: "Update"
        }
    	ListElement {
            page: "LanguagesPage.qml"
            title: "Language"
        }


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
            color: Config.transparency ? "transparent" : "black"
        }

        Rectangle {
            anchors.top: top_rect.bottom
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: Config.transparency ? "transparent" : "#999999"
        }
        Loader {
            id: background
            visible: Config.transparency ? false : true
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
            color: Config.transparency ? "transparent" : "black"
        }

        SilicaListView {
            id: listview
            model: settingsModel
            anchors.fill: parent
            header: PageHeader { 
                title: Config.tr("Settings") 
            }
            delegate: Item {
                height: 80
                width: parent.width
                Image {
                    id: arrow
                    source: "image://theme/icon-m-right"
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalRight
               }
               Label {
                    id: title
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.right: arrow.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: Config.tr(model.title)
                    font.pixelSize: 29
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        settings.openFile(model.page);
                    }
                }
            }
        }
    }
}
