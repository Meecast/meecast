import QtQuick 2.2
import Sailfish.Silica 1.0
import "jsUtils.js" as Utils

Page {
    id: settings
    allowedOrientations: Utils.decideOrientation();
    property int margin: Theme.paddingSmall
    property string header_title: Config.tr("Settings")
    property variant silicalistview_model: settingsModel
    function openFile(file) {
        pageStack.push(Qt.resolvedUrl(file))
    }
    Connections {
        target: Config
        onLanguageChanged: {
            console.log("onLanguageChanged")
            settingsModel.clear()
            add_elements_to_model()
            silicalistview_model = settingsModel
            header_title = Config.tr("Settings")
        }
    }
    function add_elements_to_model(){
        settingsModel.append({ page: "StationsPage.qml", title: "Manage locations" })
        settingsModel.append({ page: "UnitsPage.qml", title: "Measurement units" })
        settingsModel.append({ page: "VisualsPage.qml", title: "Appearance" })
        settingsModel.append({ page: "UpdatePage.qml", title: "Update" })
        settingsModel.append({ page: "LanguagesPage.qml", title: "Language" })
    }
    ListModel {
        id: settingsModel
        Component.onCompleted: add_elements_to_model()
    }

    Rectangle{
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
            id: background
            anchors.top: top_rect.bottom
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

        SilicaListView {
            id: listview
            //model: settingsModel
            model: silicalistview_model
            anchors.fill: parent
            header: PageHeader { 
                title: header_title
            }
            delegate: Item {
                height: Theme.itemSizeSmall
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
                    font.pixelSize: Theme.fontSizeMedium
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
